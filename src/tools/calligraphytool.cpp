#include "calligraphytool.h"
#include "../canvas.h"
#include "../layermanager.h"
#include "../layer.h"
#include "../commandhistory.h"
#include "../commands/drawcommand.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

CalligraphyTool::CalligraphyTool(Canvas *canvas, QObject *parent)
    : Tool(canvas, parent)
    , m_angle(45.0)
    , m_widthRatio(0.3)
{
    m_size = 20;
}

CalligraphyTool::~CalligraphyTool()
{
}

void CalligraphyTool::mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_lastPoint = pos;
        
        Layer *currentLayer = m_canvas->getLayerManager()->getCurrentLayer();
        if (currentLayer && !currentLayer->isLocked()) {
            m_beforeImage = currentLayer->getImage()->copy();
        }
        
        m_strokeBuffer = QImage(m_canvas->getDocumentSize(), QImage::Format_ARGB32);
        m_strokeBuffer.fill(Qt::transparent);
        
        drawCalligraphyStroke(pos, pos, pressure);
    }
}

void CalligraphyTool::mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (m_isDrawing && (event->buttons() & Qt::LeftButton)) {
        drawCalligraphyStroke(m_lastPoint, pos, pressure);
        m_lastPoint = pos;
        if (m_canvas) {
            m_canvas->update();
        }
    }
}

void CalligraphyTool::mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton && m_isDrawing) {
        m_isDrawing = false;
        
        Layer *currentLayer = m_canvas->getLayerManager()->getCurrentLayer();
        if (currentLayer && !currentLayer->isLocked()) {
            QPainter painter(currentLayer->getImage());
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
            painter.drawImage(0, 0, m_strokeBuffer);
            painter.end();
            
            if (m_canvas->getCommandHistory()) {
                QImage afterImage = currentLayer->getImage()->copy();
                m_canvas->getCommandHistory()->push(
                    new DrawCommand(currentLayer, m_beforeImage, afterImage)
                );
            }
        }
        
        m_strokeBuffer = QImage();
        
        if (m_canvas) {
            m_canvas->update();
        }
    }
}

void CalligraphyTool::drawOverlay(QPainter &painter)
{
    if (m_isDrawing && !m_strokeBuffer.isNull()) {
        painter.save();
        painter.drawImage(0, 0, m_strokeBuffer);
        painter.restore();
    }
}

void CalligraphyTool::drawCalligraphyStroke(const QPoint &from, const QPoint &to, qreal pressure)
{
    if (!m_canvas) return;
    
    QPainter painter(&m_strokeBuffer);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setOpacity(m_opacity);
    
    QColor color = m_canvas->getPrimaryColor();
    
    qreal angleRad = m_angle * M_PI / 180.0;
    
    QPointF direction = to - from;
    qreal strokeAngle = qAtan2(direction.y(), direction.x());
    
    qreal angleDiff = qAbs(strokeAngle - angleRad);
    while (angleDiff > M_PI) angleDiff -= M_PI;
    if (angleDiff > M_PI / 2) angleDiff = M_PI - angleDiff;
    
    qreal widthFactor = m_widthRatio + (1.0 - m_widthRatio) * qAbs(qCos(angleDiff));
    qreal effectiveWidth = m_size * widthFactor * pressure;
    
    QPen pen(color);
    pen.setWidth(effectiveWidth);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    
    painter.setPen(pen);
    painter.drawLine(from, to);
    painter.end();
}
