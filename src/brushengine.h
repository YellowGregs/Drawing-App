#ifndef BRUSHENGINE_H
#define BRUSHENGINE_H

#include <QImage>
#include <QColor>
#include <QPoint>

class BrushEngine
{
public:
    struct BrushSettings {
        int size;
        qreal hardness;
        qreal opacity;
        qreal spacing;
        bool pressureSensitive;
        bool sizeJitter;
        bool opacityJitter;
        qreal minPressure;
        qreal maxPressure;
    };

    BrushEngine();
    
    void setBrushSettings(const BrushSettings &settings);
    BrushSettings getBrushSettings() const { return m_settings; }
    
    QImage createBrushDab(int size, qreal hardness);
    void drawDab(QImage &target, const QPoint &pos, const QColor &color, qreal pressure);
    void drawStroke(QImage &target, const QPoint &from, const QPoint &to, 
                   const QColor &color, qreal pressure);

private:
    BrushSettings m_settings;
};

#endif // BRUSHENGINE_H
