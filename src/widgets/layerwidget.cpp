#include "layerwidget.h"
#include "../layermanager.h"
#include "../layer.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QListWidget>

LayerWidget::LayerWidget(LayerManager *layerManager, QWidget *parent)
    : QWidget(parent)
    , m_layerManager(layerManager)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Layer list
    m_layerList = new QListWidget(this);
    connect(m_layerList, &QListWidget::currentRowChanged, this, &LayerWidget::onLayerSelectionChanged);
    mainLayout->addWidget(m_layerList);
    
    // Opacity slider
    QHBoxLayout *opacityLayout = new QHBoxLayout();
    opacityLayout->addWidget(new QLabel(tr("Opacity:")));
    m_opacitySlider = new QSlider(Qt::Horizontal, this);
    m_opacitySlider->setRange(0, 100);
    m_opacitySlider->setValue(100);
    connect(m_opacitySlider, &QSlider::valueChanged, this, &LayerWidget::onOpacityChanged);
    opacityLayout->addWidget(m_opacitySlider);
    mainLayout->addLayout(opacityLayout);
    
    // Layer controls
    QHBoxLayout *controlLayout = new QHBoxLayout();
    
    m_visibilityButton = new QPushButton(tr("V"), this);
    m_visibilityButton->setCheckable(true);
    m_visibilityButton->setChecked(true);
    m_visibilityButton->setToolTip(tr("Toggle Visibility"));
    connect(m_visibilityButton, &QPushButton::clicked, this, &LayerWidget::onVisibilityToggled);
    controlLayout->addWidget(m_visibilityButton);
    
    m_lockButton = new QPushButton(tr("L"), this);
    m_lockButton->setCheckable(true);
    m_lockButton->setToolTip(tr("Toggle Lock"));
    connect(m_lockButton, &QPushButton::clicked, this, &LayerWidget::onLockToggled);
    controlLayout->addWidget(m_lockButton);
    
    mainLayout->addLayout(controlLayout);
    
    // Action buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_addButton = new QPushButton(tr("+"), this);
    m_addButton->setToolTip(tr("Add Layer"));
    connect(m_addButton, &QPushButton::clicked, this, &LayerWidget::addLayer);
    buttonLayout->addWidget(m_addButton);
    
    m_duplicateButton = new QPushButton(tr("⎘"), this);
    m_duplicateButton->setToolTip(tr("Duplicate Layer"));
    connect(m_duplicateButton, &QPushButton::clicked, this, &LayerWidget::duplicateLayer);
    buttonLayout->addWidget(m_duplicateButton);
    
    m_deleteButton = new QPushButton(tr("−"), this);
    m_deleteButton->setToolTip(tr("Delete Layer"));
    connect(m_deleteButton, &QPushButton::clicked, this, &LayerWidget::deleteLayer);
    buttonLayout->addWidget(m_deleteButton);
    
    m_mergeButton = new QPushButton(tr("⬇"), this);
    m_mergeButton->setToolTip(tr("Merge Down"));
    connect(m_mergeButton, &QPushButton::clicked, this, &LayerWidget::mergeDown);
    buttonLayout->addWidget(m_mergeButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Connect to layer manager signals
    connect(m_layerManager, &LayerManager::layersChanged, this, &LayerWidget::updateLayerList);
    connect(m_layerManager, &LayerManager::currentLayerChanged, this, &LayerWidget::updateLayerList);
    
    updateLayerList();
}

void LayerWidget::updateLayerList()
{
    m_layerList->blockSignals(true);
    m_layerList->clear();
    
    for (int i = m_layerManager->getLayerCount() - 1; i >= 0; --i) {
        Layer *layer = m_layerManager->getLayer(i);
        if (layer) {
            QString text = layer->getName();
            if (!layer->isVisible()) text += " (hidden)";
            if (layer->isLocked()) text += " (locked)";
            m_layerList->addItem(text);
        }
    }
    
    int currentIndex = m_layerManager->getLayerCount() - 1 - m_layerManager->getCurrentLayerIndex();
    m_layerList->setCurrentRow(currentIndex);
    
    Layer *currentLayer = m_layerManager->getCurrentLayer();
    if (currentLayer) {
        m_opacitySlider->blockSignals(true);
        m_opacitySlider->setValue(currentLayer->getOpacity() * 100);
        m_opacitySlider->blockSignals(false);
        
        m_visibilityButton->setChecked(currentLayer->isVisible());
        m_lockButton->setChecked(currentLayer->isLocked());
    }
    
    m_layerList->blockSignals(false);
}

void LayerWidget::onLayerSelectionChanged()
{
    int selectedRow = m_layerList->currentRow();
    if (selectedRow >= 0) {
        int layerIndex = m_layerManager->getLayerCount() - 1 - selectedRow;
        m_layerManager->setCurrentLayer(layerIndex);
    }
}

void LayerWidget::onOpacityChanged(int value)
{
    Layer *currentLayer = m_layerManager->getCurrentLayer();
    if (currentLayer) {
        currentLayer->setOpacity(value / 100.0);
        emit m_layerManager->layersChanged();
    }
}

void LayerWidget::onVisibilityToggled()
{
    Layer *currentLayer = m_layerManager->getCurrentLayer();
    if (currentLayer) {
        currentLayer->setVisible(m_visibilityButton->isChecked());
        emit m_layerManager->layersChanged();
        updateLayerList();
    }
}

void LayerWidget::onLockToggled()
{
    Layer *currentLayer = m_layerManager->getCurrentLayer();
    if (currentLayer) {
        currentLayer->setLocked(m_lockButton->isChecked());
        updateLayerList();
    }
}

void LayerWidget::addLayer()
{
    m_layerManager->addLayer();
}

void LayerWidget::deleteLayer()
{
    m_layerManager->deleteCurrentLayer();
}

void LayerWidget::duplicateLayer()
{
    m_layerManager->duplicateCurrentLayer();
}

void LayerWidget::mergeDown()
{
    m_layerManager->mergeDown();
}
