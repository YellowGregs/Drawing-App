#include "linetool.h"
#include "../canvas.h"
#include "../layermanager.h"
#include "../layer.h"
#include <QPainter>

LineTool::LineTool(Canvas *canvas, QObject *parent)
    : Tool(canvas, parent)
{
    m_size = 2;
}

void LineTool::mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_startPoint = pos;
        m_endPoint = pos;
    }
}

void LineTool::mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (m_isDrawing) {
        m_endPoint = pos;
    }
}

void LineTool::mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton && m_isDrawing) {
        m_isDrawing = false;
        m_endPoint = pos;
        
        Layer *currentLayer = m_canvas->getLayerManager()->getCurrentLayer();
        if (currentLayer && !currentLayer->isLocked()) {
            QPainter painter(currentLayer->getImage());
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(QPen(m_canvas->getPrimaryColor(), m_size, Qt::SolidLine, Qt::RoundCap));
            painter.setOpacity(m_opacity);
            painter.drawLine(m_startPoint, m_endPoint);
        }
    }
}

void LineTool::drawOverlay(QPainter &painter)
{
    if (m_isDrawing) {
        painter.save();
        painter.setPen(QPen(Qt::black, 1, Qt::DashLine));
        painter.drawLine(m_startPoint, m_endPoint);
        painter.restore();
    }
}
