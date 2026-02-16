#include "selecttool.h"
#include "../canvas.h"
#include <QPainter>

SelectTool::SelectTool(Canvas *canvas, QObject *parent)
    : Tool(canvas, parent)
    , m_selectMode(Rectangular)
{
}

void SelectTool::mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_startPoint = pos;
        m_endPoint = pos;
    }
}

void SelectTool::mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (m_isDrawing) {
        m_endPoint = pos;
    }
}

void SelectTool::mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton && m_isDrawing) {
        m_isDrawing = false;
        m_endPoint = pos;
        // TODO: Create actual selection
    }
}

void SelectTool::drawOverlay(QPainter &painter)
{
    if (m_isDrawing) {
        painter.save();
        painter.setPen(QPen(Qt::black, 1, Qt::DashLine));
        
        QRect rect(m_startPoint, m_endPoint);
        
        switch (m_selectMode) {
            case Rectangular:
                painter.drawRect(rect.normalized());
                break;
            case Elliptical:
                painter.drawEllipse(rect.normalized());
                break;
            case Lasso:
            case MagicWand:
                // TODO: Implement
                break;
        }
        
        painter.restore();
    }
}
