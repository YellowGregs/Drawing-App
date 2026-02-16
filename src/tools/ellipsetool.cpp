#include "ellipsetool.h"
#include "../canvas.h"
#include "../layermanager.h"
#include "../layer.h"
#include <QPainter>

EllipseTool::EllipseTool(Canvas *canvas, QObject *parent)
    : Tool(canvas, parent)
    , m_filled(false)
{
    m_size = 2;
}

void EllipseTool::mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_startPoint = pos;
        m_endPoint = pos;
    }
}

void EllipseTool::mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (m_isDrawing) {
        m_endPoint = pos;
    }
}

void EllipseTool::mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton && m_isDrawing) {
        m_isDrawing = false;
        m_endPoint = pos;
        
        Layer *currentLayer = m_canvas->getLayerManager()->getCurrentLayer();
        if (currentLayer && !currentLayer->isLocked()) {
            QPainter painter(currentLayer->getImage());
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setOpacity(m_opacity);
            
            QRect rect(m_startPoint, m_endPoint);
            
            if (m_filled) {
                painter.setPen(Qt::NoPen);
                painter.setBrush(m_canvas->getPrimaryColor());
                painter.drawEllipse(rect.normalized());
            } else {
                painter.setPen(QPen(m_canvas->getPrimaryColor(), m_size));
                painter.setBrush(Qt::NoBrush);
                painter.drawEllipse(rect.normalized());
            }
        }
    }
}

void EllipseTool::drawOverlay(QPainter &painter)
{
    if (m_isDrawing) {
        painter.save();
        painter.setPen(QPen(Qt::black, 1, Qt::DashLine));
        QRect rect(m_startPoint, m_endPoint);
        painter.drawEllipse(rect.normalized());
        painter.restore();
    }
}
