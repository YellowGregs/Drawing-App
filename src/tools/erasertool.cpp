#include "erasertool.h"
#include "../canvas.h"
#include "../layermanager.h"
#include "../layer.h"
#include <QPainter>
#include <QtMath>

EraserTool::EraserTool(Canvas *canvas, QObject *parent)
    : Tool(canvas, parent)
{
    m_size = 20;
}

void EraserTool::mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_lastPoint = pos;
        erase(pos, pressure);
    }
}

void EraserTool::mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (m_isDrawing && (event->buttons() & Qt::LeftButton)) {
        eraseStroke(m_lastPoint, pos, pressure);
        m_lastPoint = pos;
    }
}

void EraserTool::mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = false;
    }
}

void EraserTool::erase(const QPoint &pos, qreal pressure)
{
    Layer *currentLayer = m_canvas->getLayerManager()->getCurrentLayer();
    if (!currentLayer || currentLayer->isLocked()) {
        return;
    }
    
    int eraserSize = m_size * pressure;
    if (eraserSize < 1) eraserSize = 1;
    
    QPainter painter(currentLayer->getImage());
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawEllipse(pos, eraserSize / 2, eraserSize / 2);
}

void EraserTool::eraseStroke(const QPoint &from, const QPoint &to, qreal pressure)
{
    qreal distance = qSqrt(qPow(to.x() - from.x(), 2) + qPow(to.y() - from.y(), 2));
    int steps = qMax(1, (int)(distance * (1.0 / m_spacing)));
    
    for (int i = 0; i <= steps; ++i) {
        qreal t = (qreal)i / steps;
        QPoint pos(
            from.x() + t * (to.x() - from.x()),
            from.y() + t * (to.y() - from.y())
        );
        erase(pos, pressure);
    }
    
    if (m_canvas) {
        m_canvas->update();
    }
}
