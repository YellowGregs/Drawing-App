#ifndef COLORADJUSTFILTER_H
#define COLORADJUSTFILTER_H

#include "filterbase.h"

class ColorAdjustFilter : public FilterBase
{
public:
    ColorAdjustFilter();
    
    QString getName() const override { return "Color Adjust"; }
    QImage apply(const QImage &source) override;
    
    void setBrightness(int brightness) { m_brightness = brightness; }
    void setContrast(int contrast) { m_contrast = contrast; }
    void setHue(int hue) { m_hue = hue; }
    void setSaturation(int saturation) { m_saturation = saturation; }
    
    int getBrightness() const { return m_brightness; }
    int getContrast() const { return m_contrast; }
    int getHue() const { return m_hue; }
    int getSaturation() const { return m_saturation; }

private:
    int m_brightness;  // -255 to 255
    int m_contrast;    // -100 to 100
    int m_hue;         // -180 to 180
    int m_saturation;  // -100 to 100
};

#endif // COLORADJUSTFILTER_H
