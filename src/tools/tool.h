#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QMouseEvent>
#include <QPainter>
#include <QColor>
#include <QIcon>

class Canvas;

class Tool : public QObject
{
    Q_OBJECT

public:
    explicit Tool(Canvas *canvas, QObject *parent = nullptr);
    virtual ~Tool();

    // Event handlers
    virtual void mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) = 0;
    virtual void mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) = 0;
    virtual void mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) = 0;
    
    // Drawing
    virtual void drawOverlay(QPainter &painter);
    
    // Tool properties
    virtual QString getName() const = 0;
    virtual QIcon getIcon() const { return QIcon(); }
    
    // Common properties
    void setSize(int size) { m_size = size; }
    int getSize() const { return m_size; }
    
    void setOpacity(qreal opacity) { m_opacity = qBound(0.0, opacity, 1.0); }
    qreal getOpacity() const { return m_opacity; }
    
    void setHardness(qreal hardness) { m_hardness = qBound(0.0, hardness, 1.0); }
    qreal getHardness() const { return m_hardness; }
    
    void setSpacing(qreal spacing) { m_spacing = spacing; }
    qreal getSpacing() const { return m_spacing; }

signals:
    void propertyChanged();

protected:
    Canvas *m_canvas;
    int m_size;
    qreal m_opacity;
    qreal m_hardness;
    qreal m_spacing;
    bool m_isDrawing;
    QPoint m_lastPoint;
};

#endif // TOOL_H
