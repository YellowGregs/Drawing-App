#include "filterbase.h"

QImage FilterBase::clampImage(const QImage &image)
{
    QImage result = image;
    
    for (int y = 0; y < result.height(); ++y) {
        for (int x = 0; x < result.width(); ++x) {
            QColor color = result.pixelColor(x, y);
            int r = qBound(0, color.red(), 255);
            int g = qBound(0, color.green(), 255);
            int b = qBound(0, color.blue(), 255);
            result.setPixelColor(x, y, QColor(r, g, b, color.alpha()));
        }
    }
    
    return result;
}
