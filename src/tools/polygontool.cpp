#include "polygontool.h"
#include "../canvas.h"
#include "../layermanager.h"
#include "../layer.h"
#include <QPainter>
#include <QMouseEvent>

PolygonTool::PolygonTool(Canvas *canvas, QObject *parent)
    : Tool(canvas, parent)
    , m_filled(false)
{
    m_size = 2;
}

void PolygonTool::mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton) {
        m_points.append(pos);
        m_isDrawing = true;
    } else if (event->button() == Qt::RightButton && !m_points.isEmpty()) {
        // Finish polygon
        Layer *currentLayer = m_canvas->getLayerManager()->getCurrentLayer();
        if (currentLayer && !currentLayer->isLocked()) {
            QPainter painter(currentLayer->getImage());
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setOpacity(m_opacity);
            
            QPolygon polygon(m_points);
            
            if (m_filled) {
                painter.setPen(Qt::NoPen);
                painter.setBrush(m_canvas->getPrimaryColor());
                painter.drawPolygon(polygon);
            } else {
                painter.setPen(QPen(m_canvas->getPrimaryColor(), m_size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                painter.setBrush(Qt::NoBrush);
                painter.drawPolygon(polygon);
            }
        }
        
        m_points.clear();
        m_isDrawing = false;
    }
}

void PolygonTool::mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (m_isDrawing) {
        m_currentPoint = pos;
    }
}

void PolygonTool::mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    // Not used for polygon tool
}

void PolygonTool::drawOverlay(QPainter &painter)
{
    if (m_isDrawing && !m_points.isEmpty()) {
        painter.save();
        painter.setPen(QPen(Qt::black, 1, Qt::DashLine));
        
        for (int i = 0; i < m_points.size() - 1; ++i) {
            painter.drawLine(m_points[i], m_points[i + 1]);
        }
        
        painter.drawLine(m_points.last(), m_currentPoint);
        
        painter.setPen(QPen(Qt::gray, 1, Qt::DotLine));
        painter.drawLine(m_currentPoint, m_points.first());
        
        painter.restore();
    }
}
