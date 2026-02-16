#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QColor>

class LayerManager;
class Tool;
class CommandHistory;

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();

    // Document operations
    void newDocument(int width, int height, const QColor &backgroundColor = Qt::white);
    bool loadDocument(const QString &filePath);
    bool saveDocument(const QString &filePath);
    
    // Layer management
    LayerManager* getLayerManager() const { return m_layerManager; }
    
    // Tool management
    void setCurrentTool(Tool *tool);
    Tool* getCurrentTool() const { return m_currentTool; }
    
    // Command history
    void setCommandHistory(CommandHistory *history) { m_commandHistory = history; }
    CommandHistory* getCommandHistory() const { return m_commandHistory; }
    
    // Color management
    void setPrimaryColor(const QColor &color) { m_primaryColor = color; }
    void setSecondaryColor(const QColor &color) { m_secondaryColor = color; }
    QColor getPrimaryColor() const { return m_primaryColor; }
    QColor getSecondaryColor() const { return m_secondaryColor; }
    
    // View management
    void zoomIn();
    void zoomOut();
    void zoomToFit();
    void zoomToActual();
    qreal getZoomLevel() const { return m_zoomLevel; }
    void setZoomLevel(qreal zoom);
    
    // Grid and rulers
    void setGridVisible(bool visible);
    void setRulersVisible(bool visible);
    bool isGridVisible() const { return m_showGrid; }
    bool areRulersVisible() const { return m_showRulers; }
    
    // Selection
    void clearSelection();
    
    // Document properties
    QSize getDocumentSize() const { return m_documentSize; }
    QPoint getCursorPosition() const { return m_cursorPosition; }
    
    // Composite image for display
    QImage getCompositeImage() const;

signals:
    void modified();
    void cursorPositionChanged(const QPoint &pos);
    void zoomChanged(qreal zoom);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void tabletEvent(QTabletEvent *event) override;

private:
    QPoint viewToCanvas(const QPoint &viewPoint) const;
    QPoint canvasToView(const QPoint &canvasPoint) const;
    void updateViewport();
    void drawGrid(QPainter &painter);
    void drawRulers(QPainter &painter);
    
    LayerManager *m_layerManager;
    Tool *m_currentTool;
    CommandHistory *m_commandHistory;
    
    QColor m_primaryColor;
    QColor m_secondaryColor;
    
    QSize m_documentSize;
    QPoint m_viewOffset;
    qreal m_zoomLevel;
    
    bool m_showGrid;
    bool m_showRulers;
    int m_gridSize;
    
    QPoint m_cursorPosition;
    QPoint m_lastPanPoint;
    bool m_isPanning;
    
    qreal m_tabletPressure;
    QPointF m_tabletTilt;
    
    bool m_showBrushCursor;
};

#endif // CANVAS_H
