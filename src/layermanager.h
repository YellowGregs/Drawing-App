#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QObject>
#include <QList>
#include "layer.h"

class LayerManager : public QObject
{
    Q_OBJECT

public:
    explicit LayerManager(const QSize &documentSize, QObject *parent = nullptr);
    ~LayerManager();

    Layer* addLayer(const QString &name = QString());
    void insertLayer(int index, Layer *layer);
    void removeLayer(int index);
    void deleteCurrentLayer();
    Layer* duplicateCurrentLayer();
    
    Layer* getLayer(int index) const;
    Layer* getCurrentLayer() const;
    int getCurrentLayerIndex() const { return m_currentLayerIndex; }
    int getLayerCount() const { return m_layers.count(); }
    
    void setCurrentLayer(int index);
    void moveLayer(int fromIndex, int toIndex);
    void mergeDown();
    void flattenImage();
    
    QImage getCompositeImage() const;
    
    QSize getDocumentSize() const { return m_documentSize; }

signals:
    void layersChanged();
    void currentLayerChanged(int index);

private:
    QList<Layer*> m_layers;
    int m_currentLayerIndex;
    QSize m_documentSize;
    int m_layerCounter;
};

#endif // LAYERMANAGER_H
