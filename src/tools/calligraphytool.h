#ifndef CALLIGRAPHYTOOL_H
#define CALLIGRAPHYTOOL_H

#include "tool.h"
#include <QImage>

class CalligraphyTool : public Tool
{
    Q_OBJECT

public:
    explicit CalligraphyTool(Canvas *canvas, QObject *parent = nullptr);
    ~CalligraphyTool();

    void mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void drawOverlay(QPainter &painter) override;
    
    QString getName() const override { return tr("Calligraphy"); }
    
    void setAngle(qreal angle) { m_angle = angle; }
    qreal getAngle() const { return m_angle; }
    
    void setWidthRatio(qreal ratio) { m_widthRatio = qBound(0.1, ratio, 1.0); }
    qreal getWidthRatio() const { return m_widthRatio; }

private:
    void drawCalligraphyStroke(const QPoint &from, const QPoint &to, qreal pressure);
    
    QImage m_strokeBuffer;
    QImage m_beforeImage;
    qreal m_angle;        // Pen angle in degrees
    qreal m_widthRatio;   // Width ratio (narrow/wide)
};

#endif // CALLIGRAPHYTOOL_H
