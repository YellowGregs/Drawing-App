#include "spraytool.h"
#include "../canvas.h"
#include "../layermanager.h"
#include "../layer.h"
#include "../commandhistory.h"
#include "../commands/drawcommand.h"
#include <QPainter>
#include <QtMath>
#include <QRandomGenerator>

SprayTool::SprayTool(Canvas *canvas, QObject *parent)
    : Tool(canvas, parent)
    , m_density(0.5)
    , m_currentPressure(1.0)
{
    m_size = 30;
    m_sprayTimer = new QTimer(this);
    m_sprayTimer->setInterval(50); // Spray every 50ms
    connect(m_sprayTimer, &QTimer::timeout, this, &SprayTool::sprayPaint);
}

SprayTool::~SprayTool()
{
}

void SprayTool::mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_currentPos = pos;
        m_currentPressure = pressure;
        
        // Store the before image for undo
        Layer *currentLayer = m_canvas->getLayerManager()->getCurrentLayer();
        if (currentLayer && !currentLayer->isLocked()) {
            m_beforeImage = currentLayer->getImage()->copy();
        }
        
        m_strokeBuffer = QImage(m_canvas->getDocumentSize(), QImage::Format_ARGB32);
        m_strokeBuffer.fill(Qt::transparent);
        
        spraySinglePoint(pos, pressure);
        m_sprayTimer->start();
    }
}

void SprayTool::mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (m_isDrawing) {
        m_currentPos = pos;
        m_currentPressure = pressure;
    }
}

void SprayTool::mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton && m_isDrawing) {
        m_isDrawing = false;
        m_sprayTimer->stop();
        
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

void SprayTool::drawOverlay(QPainter &painter)
{
    if (m_isDrawing && !m_strokeBuffer.isNull()) {
        painter.save();
        painter.drawImage(0, 0, m_strokeBuffer);
        painter.restore();
    }
}

void SprayTool::sprayPaint()
{
    if (m_isDrawing) {
        spraySinglePoint(m_currentPos, m_currentPressure);
        if (m_canvas) {
            m_canvas->update();
        }
    }
}

void SprayTool::spraySinglePoint(const QPoint &pos, qreal pressure)
{
    if (!m_canvas) return;
    
    QPainter painter(&m_strokeBuffer);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setOpacity(m_opacity);
    
    QColor color = m_canvas->getPrimaryColor();
    painter.setPen(color);
    painter.setBrush(color);
    
    int radius = m_size * pressure;
    int particleCount = m_density * 100;
    
    QRandomGenerator *rng = QRandomGenerator::global();
    
    for (int i = 0; i < particleCount; ++i) {
        // Generate random point within circle
        qreal angle = rng->bounded(360.0) * M_PI / 180.0;
        qreal distance = rng->bounded(radius);
        
        int x = pos.x() + distance * qCos(angle);
        int y = pos.y() + distance * qSin(angle);
        
        // Draw small dot
        painter.drawEllipse(QPoint(x, y), 1, 1);
    }
    
    painter.end();
}
