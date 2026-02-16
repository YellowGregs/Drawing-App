#include "filltool.h"
#include "../canvas.h"
#include "../layermanager.h"
#include "../layer.h"
#include <QQueue>
#include <QtMath>

FillTool::FillTool(Canvas *canvas, QObject *parent)
    : Tool(canvas, parent)
    , m_tolerance(10)
{
}

void FillTool::mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton) {
        Layer *currentLayer = m_canvas->getLayerManager()->getCurrentLayer();
        if (currentLayer && !currentLayer->isLocked()) {
            QImage *image = currentLayer->getImage();
            
            if (pos.x() >= 0 && pos.x() < image->width() &&
                pos.y() >= 0 && pos.y() < image->height()) {
                
                QColor targetColor = image->pixelColor(pos);
                QColor fillColor = m_canvas->getPrimaryColor();
                
                if (!colorMatch(targetColor, fillColor, 0)) {
                    floodFill(image, pos, fillColor, targetColor);
                    if (m_canvas) {
                        m_canvas->update();
                    }
                }
            }
        }
    }
}

void FillTool::mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    // Not used for fill tool
}

void FillTool::mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    // Not used for fill tool
}

void FillTool::floodFill(QImage *image, const QPoint &pos, const QColor &fillColor, const QColor &targetColor)
{
    if (pos.x() < 0 || pos.x() >= image->width() ||
        pos.y() < 0 || pos.y() >= image->height()) {
        return;
    }
    
    QQueue<QPoint> queue;
    queue.enqueue(pos);
    
    QImage visited(image->size(), QImage::Format_Mono);
    visited.fill(0);
    
    while (!queue.isEmpty()) {
        QPoint p = queue.dequeue();
        
        if (p.x() < 0 || p.x() >= image->width() ||
            p.y() < 0 || p.y() >= image->height()) {
            continue;
        }
        
        if (visited.pixelIndex(p.x(), p.y()) != 0) {
            continue;
        }
        
        QColor currentColor = image->pixelColor(p);
        if (!colorMatch(currentColor, targetColor, m_tolerance)) {
            continue;
        }
        
        image->setPixelColor(p, fillColor);
        visited.setPixel(p.x(), p.y(), 1);
        
        queue.enqueue(QPoint(p.x() + 1, p.y()));
        queue.enqueue(QPoint(p.x() - 1, p.y()));
        queue.enqueue(QPoint(p.x(), p.y() + 1));
        queue.enqueue(QPoint(p.x(), p.y() - 1));
    }
}

bool FillTool::colorMatch(const QColor &c1, const QColor &c2, int tolerance)
{
    int dr = qAbs(c1.red() - c2.red());
    int dg = qAbs(c1.green() - c2.green());
    int db = qAbs(c1.blue() - c2.blue());
    int da = qAbs(c1.alpha() - c2.alpha());
    
    return (dr <= tolerance && dg <= tolerance && db <= tolerance && da <= tolerance);
}
