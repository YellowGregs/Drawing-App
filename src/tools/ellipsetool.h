#ifndef ELLIPSETOOL_H
#define ELLIPSETOOL_H

#include "tool.h"

class EllipseTool : public Tool
{
    Q_OBJECT

public:
    explicit EllipseTool(Canvas *canvas, QObject *parent = nullptr);
    
    void mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void drawOverlay(QPainter &painter) override;
    
    QString getName() const override { return tr("Ellipse"); }
    
    void setFilled(bool filled) { m_filled = filled; }
    bool isFilled() const { return m_filled; }

private:
    QPoint m_startPoint;
    QPoint m_endPoint;
    bool m_filled;
};

#endif // ELLIPSETOOL_H
