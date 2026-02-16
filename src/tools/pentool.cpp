#include "pentool.h"
#include "../canvas.h"
#include "../layermanager.h"
#include "../layer.h"
#include <QPainter>

PenTool::PenTool(Canvas *canvas, QObject *parent)
    : Tool(canvas, parent)
{
    m_size = 2;
}

void PenTool::mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_lastPoint = pos;
    }
}

void PenTool::mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (m_isDrawing && (event->buttons() & Qt::LeftButton)) {
        drawLine(m_lastPoint, pos);
        m_lastPoint = pos;
    }
}

void PenTool::mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = false;
    }
}

void PenTool::drawLine(const QPoint &from, const QPoint &to)
{
    Layer *currentLayer = m_canvas->getLayerManager()->getCurrentLayer();
    if (!currentLayer || currentLayer->isLocked()) {
        return;
    }
    
    QPainter painter(currentLayer->getImage());
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(m_canvas->getPrimaryColor(), m_size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setOpacity(m_opacity);
    painter.drawLine(from, to);
    
    if (m_canvas) {
        m_canvas->update();
    }
}
