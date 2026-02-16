#include "layer.h"
#include <QPainter>

Layer::Layer(const QSize &size, const QString &name)
    : m_image(size, QImage::Format_ARGB32)
    , m_name(name)
    , m_opacity(1.0)
    , m_visible(true)
    , m_locked(false)
    , m_blendMode(Normal)
{
    m_image.fill(Qt::transparent);
}

Layer::~Layer()
{
}

void Layer::clear()
{
    m_image.fill(Qt::transparent);
}

void Layer::fill(const QColor &color)
{
    m_image.fill(color);
}

Layer* Layer::duplicate() const
{
    Layer *newLayer = new Layer(m_image.size(), m_name + " copy");
    *newLayer->getImage() = m_image.copy();
    newLayer->setOpacity(m_opacity);
    newLayer->setVisible(m_visible);
    newLayer->setBlendMode(m_blendMode);
    return newLayer;
}

void Layer::composite(QImage &destination, const QImage &source, 
                     BlendMode mode, qreal opacity)
{
    if (destination.size() != source.size()) {
        return;
    }
    
    QPainter painter(&destination);
    
    switch (mode) {
        case Normal:
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
            break;
        case Multiply:
            painter.setCompositionMode(QPainter::CompositionMode_Multiply);
            break;
        case Screen:
            painter.setCompositionMode(QPainter::CompositionMode_Screen);
            break;
        case Overlay:
            painter.setCompositionMode(QPainter::CompositionMode_Overlay);
            break;
        case Darken:
            painter.setCompositionMode(QPainter::CompositionMode_Darken);
            break;
        case Lighten:
            painter.setCompositionMode(QPainter::CompositionMode_Lighten);
            break;
        case ColorDodge:
            painter.setCompositionMode(QPainter::CompositionMode_ColorDodge);
            break;
        case ColorBurn:
            painter.setCompositionMode(QPainter::CompositionMode_ColorBurn);
            break;
        case HardLight:
            painter.setCompositionMode(QPainter::CompositionMode_HardLight);
            break;
        case SoftLight:
            painter.setCompositionMode(QPainter::CompositionMode_SoftLight);
            break;
        case Difference:
            painter.setCompositionMode(QPainter::CompositionMode_Difference);
            break;
        case Exclusion:
            painter.setCompositionMode(QPainter::CompositionMode_Exclusion);
            break;
    }
    
    painter.setOpacity(opacity);
    painter.drawImage(0, 0, source);
}
