#include "blurfilter.h"
#include <QtMath>

BlurFilter::BlurFilter(int radius)
    : m_radius(radius)
{
}

QImage BlurFilter::apply(const QImage &source)
{
    QImage result = source;
    
    //box blur
    for (int y = 0; y < source.height(); ++y) {
        for (int x = 0; x < source.width(); ++x) {
            int r = 0, g = 0, b = 0, a = 0;
            int count = 0;
            
            for (int dy = -m_radius; dy <= m_radius; ++dy) {
                for (int dx = -m_radius; dx <= m_radius; ++dx) {
                    int nx = x + dx;
                    int ny = y + dy;
                    
                    if (nx >= 0 && nx < source.width() && 
                        ny >= 0 && ny < source.height()) {
                        QColor c = source.pixelColor(nx, ny);
                        r += c.red();
                        g += c.green();
                        b += c.blue();
                        a += c.alpha();
                        count++;
                    }
                }
            }
            
            if (count > 0) {
                result.setPixelColor(x, y, QColor(r/count, g/count, b/count, a/count));
            }
        }
    }
    
    return result;
}
