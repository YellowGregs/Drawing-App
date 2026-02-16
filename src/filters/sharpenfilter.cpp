#include "sharpenfilter.h"

SharpenFilter::SharpenFilter(qreal amount)
    : m_amount(amount)
{
}

QImage SharpenFilter::apply(const QImage &source)
{
    QImage result = source;
    
    // Sharpen kernel
    qreal kernel[3][3] = {
        { 0, -1 * m_amount,  0},
        {-1 * m_amount, 1 + 4 * m_amount, -1 * m_amount},
        { 0, -1 * m_amount,  0}
    };
    
    for (int y = 1; y < source.height() - 1; ++y) {
        for (int x = 1; x < source.width() - 1; ++x) {
            qreal r = 0, g = 0, b = 0;
            
            for (int ky = 0; ky < 3; ++ky) {
                for (int kx = 0; kx < 3; ++kx) {
                    QColor c = source.pixelColor(x + kx - 1, y + ky - 1);
                    r += c.red() * kernel[ky][kx];
                    g += c.green() * kernel[ky][kx];
                    b += c.blue() * kernel[ky][kx];
                }
            }
            
            int nr = qBound(0, (int)r, 255);
            int ng = qBound(0, (int)g, 255);
            int nb = qBound(0, (int)b, 255);
            
            QColor originalColor = source.pixelColor(x, y);
            result.setPixelColor(x, y, QColor(nr, ng, nb, originalColor.alpha()));
        }
    }
    
    return result;
}
