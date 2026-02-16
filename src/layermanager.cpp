#include "layermanager.h"
#include <QPainter>

LayerManager::LayerManager(const QSize &documentSize, QObject *parent)
    : QObject(parent)
    , m_currentLayerIndex(-1)
    , m_documentSize(documentSize)
    , m_layerCounter(1)
{
}

LayerManager::~LayerManager()
{
    qDeleteAll(m_layers);
}

Layer* LayerManager::addLayer(const QString &name)
{
    QString layerName = name.isEmpty() ? QString("Layer %1").arg(m_layerCounter++) : name;
    Layer *layer = new Layer(m_documentSize, layerName);
    m_layers.append(layer);
    m_currentLayerIndex = m_layers.count() - 1;
    
    emit layersChanged();
    emit currentLayerChanged(m_currentLayerIndex);
    
    return layer;
}

void LayerManager::insertLayer(int index, Layer *layer)
{
    if (index < 0 || index > m_layers.count()) {
        return;
    }
    
    m_layers.insert(index, layer);
    m_currentLayerIndex = index;
    
    emit layersChanged();
    emit currentLayerChanged(m_currentLayerIndex);
}

void LayerManager::removeLayer(int index)
{
    if (index < 0 || index >= m_layers.count() || m_layers.count() == 1) {
        return;
    }
    
    delete m_layers.takeAt(index);
    
    if (m_currentLayerIndex >= m_layers.count()) {
        m_currentLayerIndex = m_layers.count() - 1;
    }
    
    emit layersChanged();
    emit currentLayerChanged(m_currentLayerIndex);
}

void LayerManager::deleteCurrentLayer()
{
    removeLayer(m_currentLayerIndex);
}

Layer* LayerManager::duplicateCurrentLayer()
{
    if (m_currentLayerIndex < 0 || m_currentLayerIndex >= m_layers.count()) {
        return nullptr;
    }
    
    Layer *currentLayer = m_layers[m_currentLayerIndex];
    Layer *newLayer = currentLayer->duplicate();
    
    insertLayer(m_currentLayerIndex + 1, newLayer);
    
    return newLayer;
}

Layer* LayerManager::getLayer(int index) const
{
    if (index < 0 || index >= m_layers.count()) {
        return nullptr;
    }
    return m_layers[index];
}

Layer* LayerManager::getCurrentLayer() const
{
    return getLayer(m_currentLayerIndex);
}

void LayerManager::setCurrentLayer(int index)
{
    if (index < 0 || index >= m_layers.count()) {
        return;
    }
    
    m_currentLayerIndex = index;
    emit currentLayerChanged(m_currentLayerIndex);
}

void LayerManager::moveLayer(int fromIndex, int toIndex)
{
    if (fromIndex < 0 || fromIndex >= m_layers.count() ||
        toIndex < 0 || toIndex >= m_layers.count()) {
        return;
    }
    
    Layer *layer = m_layers.takeAt(fromIndex);
    m_layers.insert(toIndex, layer);
    
    if (m_currentLayerIndex == fromIndex) {
        m_currentLayerIndex = toIndex;
    }
    
    emit layersChanged();
}

void LayerManager::mergeDown()
{
    if (m_currentLayerIndex <= 0 || m_currentLayerIndex >= m_layers.count()) {
        return;
    }
    
    Layer *currentLayer = m_layers[m_currentLayerIndex];
    Layer *belowLayer = m_layers[m_currentLayerIndex - 1];
    
    Layer::composite(*belowLayer->getImage(), *currentLayer->getImage(),
                    currentLayer->getBlendMode(), currentLayer->getOpacity());
    
    removeLayer(m_currentLayerIndex);
    m_currentLayerIndex--;
    
    emit currentLayerChanged(m_currentLayerIndex);
}

void LayerManager::flattenImage()
{
    if (m_layers.isEmpty()) {
        return;
    }
    
    QImage flattened = getCompositeImage();
    
    while (m_layers.count() > 1) {
        delete m_layers.takeLast();
    }
    
    *m_layers[0]->getImage() = flattened;
    m_layers[0]->setName("Background");
    m_layers[0]->setOpacity(1.0);
    m_layers[0]->setBlendMode(Layer::Normal);
    
    m_currentLayerIndex = 0;
    
    emit layersChanged();
    emit currentLayerChanged(m_currentLayerIndex);
}

QImage LayerManager::getCompositeImage() const
{
    QImage composite(m_documentSize, QImage::Format_ARGB32);
    composite.fill(Qt::transparent);
    
    for (const Layer *layer : m_layers) {
        if (layer->isVisible()) {
            Layer::composite(composite, *layer->getImage(),
                           layer->getBlendMode(), layer->getOpacity());
        }
    }
    
    return composite;
}

