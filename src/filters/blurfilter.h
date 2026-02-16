#ifndef BLURFILTER_H
#define BLURFILTER_H

#include "filterbase.h"

class BlurFilter : public FilterBase
{
public:
    BlurFilter(int radius = 5);
    
    QString getName() const override { return "Blur"; }
    QImage apply(const QImage &source) override;
    
    void setRadius(int radius) { m_radius = radius; }
    int getRadius() const { return m_radius; }

private:
    int m_radius;
};

#endif // BLURFILTER_H
