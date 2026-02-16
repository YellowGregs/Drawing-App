#ifndef LAYERWIDGET_H
#define LAYERWIDGET_H

#include <QWidget>
#include <QListWidget>

class LayerManager;
class QPushButton;
class QSlider;

class LayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LayerWidget(LayerManager *layerManager, QWidget *parent = nullptr);

private slots:
    void updateLayerList();
    void onLayerSelectionChanged();
    void onOpacityChanged(int value);
    void onVisibilityToggled();
    void onLockToggled();
    void addLayer();
    void deleteLayer();
    void duplicateLayer();
    void mergeDown();

private:
    LayerManager *m_layerManager;
    QListWidget *m_layerList;
    QSlider *m_opacitySlider;
    QPushButton *m_visibilityButton;
    QPushButton *m_lockButton;
    QPushButton *m_addButton;
    QPushButton *m_deleteButton;
    QPushButton *m_duplicateButton;
    QPushButton *m_mergeButton;
};

#endif // LAYERWIDGET_H
