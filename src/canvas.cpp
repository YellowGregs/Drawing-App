#include "canvas.h"
#include "layermanager.h"
#include "tools/tool.h"
#include "commandhistory.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTabletEvent>
#include <QPainterPath>
#include <QImageReader>
#include <QImageWriter>
#include <cmath>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , m_layerManager(nullptr)
    , m_currentTool(nullptr)
    , m_commandHistory(nullptr)
    , m_primaryColor(Qt::black)
    , m_secondaryColor(Qt::white)
    , m_documentSize(800, 600)
    , m_viewOffset(0, 0)
    , m_zoomLevel(1.0)
    , m_showGrid(false)
    , m_showRulers(false)
    , m_gridSize(20)
    , m_isPanning(false)
    , m_tabletPressure(1.0)
    , m_showBrushCursor(true)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_StaticContents);
    setAttribute(Qt::WA_OpaquePaintEvent);
    
    m_layerManager = new LayerManager(m_documentSize, this);
    
    m_layerManager->addLayer();
}

Canvas::~Canvas()
{
    delete m_currentTool;
}

void Canvas::newDocument(int width, int height, const QColor &backgroundColor)
{
    m_documentSize = QSize(width, height);
    delete m_layerManager;
    m_layerManager = new LayerManager(m_documentSize, this);
    m_layerManager->addLayer();
    
    if (backgroundColor != Qt::transparent) {
        QImage *layerImage = m_layerManager->getCurrentLayer()->getImage();
        layerImage->fill(backgroundColor);
    }
    
    m_viewOffset = QPoint(0, 0);
    m_zoomLevel = 1.0;
    
    updateViewport();
    update();
    emit modified();
}

bool Canvas::loadDocument(const QString &filePath)
{
    QImageReader reader(filePath);
    QImage image = reader.read();
    
    if (image.isNull()) {
        return false;
    }
    
    m_documentSize = image.size();
    delete m_layerManager;
    m_layerManager = new LayerManager(m_documentSize, this);
    
    Layer *layer = m_layerManager->addLayer();
    *layer->getImage() = image.convertToFormat(QImage::Format_ARGB32);
    
    m_viewOffset = QPoint(0, 0);
    zoomToFit();
    
    update();
    emit modified();
    return true;
}

bool Canvas::saveDocument(const QString &filePath)
{
    QImage composite = getCompositeImage();
    QImageWriter writer(filePath);
    return writer.write(composite);
}

void Canvas::setCurrentTool(Tool *tool)
{
    if (m_currentTool) {
        delete m_currentTool;
    }
    m_currentTool = tool;
}

void Canvas::zoomIn()
{
    setZoomLevel(m_zoomLevel * 1.2);
}

void Canvas::zoomOut()
{
    setZoomLevel(m_zoomLevel / 1.2);
}

void Canvas::zoomToFit()
{
    qreal zoomX = (qreal)width() / m_documentSize.width();
    qreal zoomY = (qreal)height() / m_documentSize.height();
    setZoomLevel(qMin(zoomX, zoomY) * 0.9);
    
    QSize scaledSize = m_documentSize * m_zoomLevel;
    m_viewOffset.setX((width() - scaledSize.width()) / 2);
    m_viewOffset.setY((height() - scaledSize.height()) / 2);
    update();
}

void Canvas::zoomToActual()
{
    setZoomLevel(1.0);
}

void Canvas::setZoomLevel(qreal zoom)
{
    m_zoomLevel = qBound(0.1, zoom, 10.0);
    update();
    emit zoomChanged(m_zoomLevel);
}

void Canvas::setGridVisible(bool visible)
{
    m_showGrid = visible;
    update();
}

void Canvas::setRulersVisible(bool visible)
{
    m_showRulers = visible;
    update();
}

void Canvas::clearSelection()
{
    // TODO: Implement selection clearing
    update();
}

QImage Canvas::getCompositeImage() const
{
    return m_layerManager->getCompositeImage();
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    
    painter.fillRect(rect(), QColor(80, 80, 80));
    
    QSize scaledSize = m_documentSize * m_zoomLevel;
    QRect canvasRect(m_viewOffset, scaledSize);
    
    int checkSize = 10;
    for (int y = 0; y < scaledSize.height(); y += checkSize) {
        for (int x = 0; x < scaledSize.width(); x += checkSize) {
            if ((x / checkSize + y / checkSize) % 2 == 0) {
                painter.fillRect(m_viewOffset.x() + x, m_viewOffset.y() + y, 
                               checkSize, checkSize, QColor(200, 200, 200));
            } else {
                painter.fillRect(m_viewOffset.x() + x, m_viewOffset.y() + y, 
                               checkSize, checkSize, Qt::white);
            }
        }
    }
    
    QImage composite = getCompositeImage();
    QImage scaled = composite.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawImage(m_viewOffset, scaled);
    
    if (m_showGrid) {
        drawGrid(painter);
    }
    
    if (m_showRulers) {
        drawRulers(painter);
    }
    
    painter.setPen(QPen(Qt::black, 1));
    painter.drawRect(canvasRect.adjusted(0, 0, -1, -1));
    
    if (m_currentTool) {
        painter.save();
        painter.translate(m_viewOffset);
        painter.scale(m_zoomLevel, m_zoomLevel);
        m_currentTool->drawOverlay(painter);
        painter.restore();
    }
    
    if (m_showBrushCursor && m_currentTool && rect().contains(mapFromGlobal(QCursor::pos()))) {
        QPoint cursorPos = mapFromGlobal(QCursor::pos());
        int brushSize = m_currentTool->getSize() * m_zoomLevel;
        
        painter.save();
        painter.setPen(QPen(Qt::white, 1));
        painter.drawEllipse(cursorPos, brushSize / 2, brushSize / 2);
        painter.setPen(QPen(Qt::black, 1, Qt::DashLine));
        painter.drawEllipse(cursorPos, brushSize / 2, brushSize / 2);
        painter.restore();
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    // Space + Left mouse button for panning
    if (event->modifiers() & Qt::ShiftModifier || event->button() == Qt::MiddleButton) {
        m_isPanning = true;
        m_lastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
        return;
    }
    
    QPoint canvasPos = viewToCanvas(event->pos());
    m_cursorPosition = canvasPos;
    
    if (m_currentTool && canvasPos.x() >= 0 && canvasPos.y() >= 0 &&
        canvasPos.x() < m_documentSize.width() && canvasPos.y() < m_documentSize.height()) {
        m_currentTool->mousePressEvent(event, canvasPos, m_tabletPressure);
        emit modified();
    }
    
    update();
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPanning) {
        QPoint delta = event->pos() - m_lastPanPoint;
        m_viewOffset += delta;
        m_lastPanPoint = event->pos();
        update();
        return;
    }
    
    QPoint canvasPos = viewToCanvas(event->pos());
    m_cursorPosition = canvasPos;
    emit cursorPositionChanged(canvasPos);
    
    if (m_currentTool && canvasPos.x() >= 0 && canvasPos.y() >= 0 &&
        canvasPos.x() < m_documentSize.width() && canvasPos.y() < m_documentSize.height()) {
        m_currentTool->mouseMoveEvent(event, canvasPos, m_tabletPressure);
    }
    
    update();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isPanning) {
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);
        return;
    }
    
    QPoint canvasPos = viewToCanvas(event->pos());
    
    if (m_currentTool) {
        m_currentTool->mouseReleaseEvent(event, canvasPos, m_tabletPressure);
    }
    
    update();
}

void Canvas::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        qreal factor = 1.0 + event->angleDelta().y() / 1200.0;
        setZoomLevel(m_zoomLevel * factor);
        event->accept();
    } else {
        event->ignore();
    }
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateViewport();
}

void Canvas::tabletEvent(QTabletEvent *event)
{
    m_tabletPressure = event->pressure();
    m_tabletTilt = QPointF(event->xTilt(), event->yTilt());
    
    QPoint canvasPos = viewToCanvas(event->pos());
    m_cursorPosition = canvasPos;
    
    if (m_currentTool && canvasPos.x() >= 0 && canvasPos.y() >= 0 &&
        canvasPos.x() < m_documentSize.width() && canvasPos.y() < m_documentSize.height()) {
        
        QMouseEvent mouseEvent(
            event->type() == QEvent::TabletPress ? QEvent::MouseButtonPress :
            event->type() == QEvent::TabletRelease ? QEvent::MouseButtonRelease :
            QEvent::MouseMove,
            event->pos(),
            Qt::LeftButton,
            Qt::LeftButton,
            event->modifiers()
        );
        
        if (event->type() == QEvent::TabletPress) {
            m_currentTool->mousePressEvent(&mouseEvent, canvasPos, m_tabletPressure);
        } else if (event->type() == QEvent::TabletMove) {
            m_currentTool->mouseMoveEvent(&mouseEvent, canvasPos, m_tabletPressure);
        } else if (event->type() == QEvent::TabletRelease) {
            m_currentTool->mouseReleaseEvent(&mouseEvent, canvasPos, m_tabletPressure);
        }
    }
    
    update();
    event->accept();
}

QPoint Canvas::viewToCanvas(const QPoint &viewPoint) const
{
    QPoint canvasPoint = viewPoint - m_viewOffset;
    canvasPoint.setX(canvasPoint.x() / m_zoomLevel);
    canvasPoint.setY(canvasPoint.y() / m_zoomLevel);
    return canvasPoint;
}

QPoint Canvas::canvasToView(const QPoint &canvasPoint) const
{
    QPoint viewPoint = canvasPoint * m_zoomLevel;
    viewPoint += m_viewOffset;
    return viewPoint;
}

void Canvas::updateViewport()
{
    if (m_documentSize.width() * m_zoomLevel < width() &&
        m_documentSize.height() * m_zoomLevel < height()) {
        QSize scaledSize = m_documentSize * m_zoomLevel;
        m_viewOffset.setX((width() - scaledSize.width()) / 2);
        m_viewOffset.setY((height() - scaledSize.height()) / 2);
    }
}

void Canvas::drawGrid(QPainter &painter)
{
    painter.save();
    painter.setPen(QPen(QColor(150, 150, 150, 100), 1));
    
    QSize scaledSize = m_documentSize * m_zoomLevel;
    int scaledGridSize = m_gridSize * m_zoomLevel;
    
    // Vertical lines
    for (int x = 0; x < scaledSize.width(); x += scaledGridSize) {
        painter.drawLine(m_viewOffset.x() + x, m_viewOffset.y(),
                        m_viewOffset.x() + x, m_viewOffset.y() + scaledSize.height());
    }
    
    // Horizontal lines
    for (int y = 0; y < scaledSize.height(); y += scaledGridSize) {
        painter.drawLine(m_viewOffset.x(), m_viewOffset.y() + y,
                        m_viewOffset.x() + scaledSize.width(), m_viewOffset.y() + y);
    }
    
    painter.restore();
}

void Canvas::drawRulers(QPainter &painter)
{
    // TODO: Implement ruler drawing
    painter.save();
    painter.fillRect(0, 0, width(), 20, QColor(240, 240, 240));
    painter.fillRect(0, 0, 20, height(), QColor(240, 240, 240));
    painter.restore();
}
