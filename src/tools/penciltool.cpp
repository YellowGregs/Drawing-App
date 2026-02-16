#include "penciltool.h"
#include "../canvas.h"
#include "../layermanager.h"
#include "../layer.h"
#include <QPainter>

PencilTool::PencilTool(Canvas *canvas, QObject *parent)
    : Tool(canvas, parent)
{
    m_size = 1;
}

void PencilTool::mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_lastPoint = pos;
        drawPixel(pos);
    }
}

void PencilTool::mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (m_isDrawing && (event->buttons() & Qt::LeftButton)) {
        drawLine(m_lastPoint, pos);
        m_lastPoint = pos;
    }
}

void PencilTool::mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = false;
    }
}

void PencilTool::drawPixel(const QPoint &pos)
{
    Layer *currentLayer = m_canvas->getLayerManager()->getCurrentLayer();
    if (!currentLayer || currentLayer->isLocked()) {
        return;
    }
    
    QImage *image = currentLayer->getImage();
    if (pos.x() >= 0 && pos.x() < image->width() &&
        pos.y() >= 0 && pos.y() < image->height()) {
        image->setPixelColor(pos, m_canvas->getPrimaryColor());
    }
}

void PencilTool::drawLine(const QPoint &from, const QPoint &to)
{
    Layer *currentLayer = m_canvas->getLayerManager()->getCurrentLayer();
    if (!currentLayer || currentLayer->isLocked()) {
        return;
    }
    
    QPainter painter(currentLayer->getImage());
    painter.setPen(QPen(m_canvas->getPrimaryColor(), m_size));
    painter.drawLine(from, to);
    
    if (m_canvas) {
        m_canvas->update();
    }
}
