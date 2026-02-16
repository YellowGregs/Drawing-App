#include "coloradjustfilter.h"
#include <QtMath>

ColorAdjustFilter::ColorAdjustFilter()
    : m_brightness(0)
    , m_contrast(0)
    , m_hue(0)
    , m_saturation(0)
{
}

QImage ColorAdjustFilter::apply(const QImage &source)
{
    QImage result = source;
    
    for (int y = 0; y < source.height(); ++y) {
        for (int x = 0; x < source.width(); ++x) {
            QColor color = source.pixelColor(x, y);
            
            // brightness
            int r = color.red() + m_brightness;
            int g = color.green() + m_brightness;
            int b = color.blue() + m_brightness;
            
            // contrast
            if (m_contrast != 0) {
                qreal factor = (259.0 * (m_contrast + 255)) / (255.0 * (259 - m_contrast));
                r = factor * (r - 128) + 128;
                g = factor * (g - 128) + 128;
                b = factor * (b - 128) + 128;
            }
            
            //Clamp RGB values
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);
            
            QColor adjustedColor(r, g, b, color.alpha());
            
            //hue/saturation adjustments
            if (m_hue != 0 || m_saturation != 0) {
                int h = adjustedColor.hue();
                int s = adjustedColor.saturation();
                int v = adjustedColor.value();
                
                h = (h + m_hue + 360) % 360;
                s = qBound(0, s + m_saturation, 255);
                
                adjustedColor.setHsv(h, s, v, color.alpha());
            }
            
            result.setPixelColor(x, y, adjustedColor);
        }
    }
    
    return result;
}
