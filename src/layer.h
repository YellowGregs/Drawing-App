#ifndef LAYER_H
#define LAYER_H

#include <QImage>
#include <QString>
#include <QPainter>

class Layer
{
public:
    enum BlendMode {
        Normal,
        Multiply,
        Screen,
        Overlay,
        Darken,
        Lighten,
        ColorDodge,
        ColorBurn,
        HardLight,
        SoftLight,
        Difference,
        Exclusion
    };

    explicit Layer(const QSize &size, const QString &name = "Layer");
    ~Layer();

    // Image access
    QImage* getImage() { return &m_image; }
    const QImage* getImage() const { return &m_image; }
    
    // Properties
    QString getName() const { return m_name; }
    void setName(const QString &name) { m_name = name; }
    
    qreal getOpacity() const { return m_opacity; }
    void setOpacity(qreal opacity) { m_opacity = qBound(0.0, opacity, 1.0); }
    
    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }
    
    bool isLocked() const { return m_locked; }
    void setLocked(bool locked) { m_locked = locked; }
    
    BlendMode getBlendMode() const { return m_blendMode; }
    void setBlendMode(BlendMode mode) { m_blendMode = mode; }
    
    // Layer operations
    void clear();
    void fill(const QColor &color);
    Layer* duplicate() const;
    
    // Compositing
    static void composite(QImage &destination, const QImage &source, 
                         BlendMode mode, qreal opacity);

private:
    QImage m_image;
    QString m_name;
    qreal m_opacity;
    bool m_visible;
    bool m_locked;
    BlendMode m_blendMode;
};

#endif // LAYER_H
