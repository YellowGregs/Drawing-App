#include "brushtool.h"
#include "../canvas.h"
#include "../layermanager.h"
#include "../layer.h"
#include "../commandhistory.h"
#include "../commands/drawcommand.h"
#include <QPainter>
#include <QtMath>

BrushTool::BrushTool(Canvas *canvas, QObject *parent)
    : Tool(canvas, parent)
{
    m_size = 20;
    m_hardness = 0.5;
    m_opacity = 1.0;
    m_brushDab = createBrushDab(m_size, m_hardness);
}

BrushTool::~BrushTool()
{
}

void BrushTool::mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_lastPoint = pos;
        
        // Store the before image for undo
        Layer *currentLayer = m_canvas->getLayerManager()->getCurrentLayer();
        if (currentLayer && !currentLayer->isLocked()) {
            m_beforeImage = currentLayer->getImage()->copy();
        }
        
        m_strokeBuffer = QImage(m_canvas->getDocumentSize(), QImage::Format_ARGB32);
        m_strokeBuffer.fill(Qt::transparent);
        
        drawDab(pos, pressure);
    }
}

void BrushTool::mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (m_isDrawing && (event->buttons() & Qt::LeftButton)) {
        drawStroke(m_lastPoint, pos, pressure);
        m_lastPoint = pos;
        if (m_canvas) {
            m_canvas->update();
        }
    }
}

void BrushTool::mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton && m_isDrawing) {
        m_isDrawing = false;
        
        Layer *currentLayer = m_canvas->getLayerManager()->getCurrentLayer();
        if (currentLayer && !currentLayer->isLocked()) {
            QPainter painter(currentLayer->getImage());
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
            painter.drawImage(0, 0, m_strokeBuffer);
            painter.end();
            
            // Create undo command
            if (m_canvas->getCommandHistory()) {
                QImage afterImage = currentLayer->getImage()->copy();
                m_canvas->getCommandHistory()->push(
                    new DrawCommand(currentLayer, m_beforeImage, afterImage)
                );
            }
        }
        
        // Clear the stroke buffer
        m_strokeBuffer = QImage();
        
        if (m_canvas) {
            m_canvas->update();
        }
    }
}

void BrushTool::drawOverlay(QPainter &painter)
{
    if (m_isDrawing && !m_strokeBuffer.isNull()) {
        painter.save();
        painter.drawImage(0, 0, m_strokeBuffer);
        painter.restore();
    }
}

void BrushTool::drawDab(const QPoint &pos, qreal pressure)
{
    if (!m_canvas) return;
    
    int dabSize = qMax(1, (int)(m_size * pressure));
    
    QImage dab = createBrushDab(dabSize, m_hardness);
    
    QPainter painter(&m_strokeBuffer);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setOpacity(m_opacity);
    
    QColor color = m_canvas->getPrimaryColor();
    
    // Apply color to the dab
    QImage coloredDab = dab;
    for (int y = 0; y < coloredDab.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(coloredDab.scanLine(y));
        for (int x = 0; x < coloredDab.width(); ++x) {
            int alpha = qAlpha(line[x]);
            line[x] = qRgba(color.red(), color.green(), color.blue(), alpha);
        }
    }
    
    painter.drawImage(pos.x() - dabSize / 2, pos.y() - dabSize / 2, coloredDab);
    painter.end();
}

void BrushTool::drawStroke(const QPoint &from, const QPoint &to, qreal pressure)
{
    qreal distance = qSqrt(qPow(to.x() - from.x(), 2) + qPow(to.y() - from.y(), 2));
    int steps = qMax(1, (int)(distance * (1.0 / m_spacing)));
    
    for (int i = 0; i <= steps; ++i) {
        qreal t = (qreal)i / steps;
        QPoint pos(
            from.x() + t * (to.x() - from.x()),
            from.y() + t * (to.y() - from.y())
        );
        drawDab(pos, pressure);
    }
}

QImage BrushTool::createBrushDab(int size, qreal hardness)
{
    QImage dab(size, size, QImage::Format_ARGB32);
    dab.fill(Qt::transparent);
    
    int center = size / 2;
    qreal radius = size / 2.0;
    
    for (int y = 0; y < size; ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(dab.scanLine(y));
        for (int x = 0; x < size; ++x) {
            qreal dx = x - center;
            qreal dy = y - center;
            qreal distance = qSqrt(dx * dx + dy * dy);
            
            int alpha = 0;
            if (distance < radius) {
                qreal normalized = distance / radius;
                
                if (normalized < hardness) {
                    alpha = 255;
                } else {
                    qreal softEdge = (1.0 - normalized) / (1.0 - hardness);
                    alpha = qBound(0, (int)(softEdge * 255), 255);
                }
            }
            
            line[x] = qRgba(255, 255, 255, alpha);
        }
    }
    
    return dab;
}
