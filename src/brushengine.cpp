#include "brushengine.h"
#include <QPainter>
#include <QtMath>
#include <QRandomGenerator>

BrushEngine::BrushEngine()
{
    m_settings.size = 20;
    m_settings.hardness = 0.5;
    m_settings.opacity = 1.0;
    m_settings.spacing = 0.25;
    m_settings.pressureSensitive = true;
    m_settings.sizeJitter = false;
    m_settings.opacityJitter = false;
    m_settings.minPressure = 0.1;
    m_settings.maxPressure = 1.0;
}

void BrushEngine::setBrushSettings(const BrushSettings &settings)
{
    m_settings = settings;
}

QImage BrushEngine::createBrushDab(int size, qreal hardness)
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

void BrushEngine::drawDab(QImage &target, const QPoint &pos, const QColor &color, qreal pressure)
{
    qreal effectivePressure = pressure;
    if (m_settings.pressureSensitive) {
        effectivePressure = m_settings.minPressure + 
                          (m_settings.maxPressure - m_settings.minPressure) * pressure;
    }
    
    int effectiveSize = m_settings.size;
    if (m_settings.pressureSensitive) {
        effectiveSize = m_settings.size * effectivePressure;
    }
    if (m_settings.sizeJitter) {
        qreal jitter = 0.8 + QRandomGenerator::global()->generateDouble() * 0.4;
        effectiveSize *= jitter;
    }
    effectiveSize = qMax(1, effectiveSize);
    
    qreal effectiveOpacity = m_settings.opacity;
    if (m_settings.pressureSensitive) {
        effectiveOpacity *= effectivePressure;
    }
    if (m_settings.opacityJitter) {
        qreal jitter = 0.8 + QRandomGenerator::global()->generateDouble() * 0.2;
        effectiveOpacity *= jitter;
    }
    
    QImage dab = createBrushDab(effectiveSize, m_settings.hardness);
    for (int y = 0; y < dab.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(dab.scanLine(y));
        for (int x = 0; x < dab.width(); ++x) {
            int alpha = qAlpha(line[x]);
            alpha = alpha * effectiveOpacity;
            line[x] = qRgba(color.red(), color.green(), color.blue(), alpha);
        }
    }
    
    QPainter painter(&target);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(pos.x() - effectiveSize / 2, pos.y() - effectiveSize / 2, dab);
}

void BrushEngine::drawStroke(QImage &target, const QPoint &from, const QPoint &to, 
                            const QColor &color, qreal pressure)
{
    qreal distance = qSqrt(qPow(to.x() - from.x(), 2) + qPow(to.y() - from.y(), 2));
    int steps = qMax(1, (int)(distance * (1.0 / m_settings.spacing)));
    
    for (int i = 0; i <= steps; ++i) {
        qreal t = (qreal)i / steps;
        QPoint pos(
            from.x() + t * (to.x() - from.x()),
            from.y() + t * (to.y() - from.y())
        );
        drawDab(target, pos, color, pressure);
    }
}
