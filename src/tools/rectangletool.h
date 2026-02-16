#ifndef RECTANGLETOOL_H
#define RECTANGLETOOL_H

#include "tool.h"

class RectangleTool : public Tool
{
    Q_OBJECT

public:
    explicit RectangleTool(Canvas *canvas, QObject *parent = nullptr);
    
    void mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void drawOverlay(QPainter &painter) override;
    
    QString getName() const override { return tr("Rectangle"); }
    
    void setFilled(bool filled) { m_filled = filled; }
    bool isFilled() const { return m_filled; }

private:
    QPoint m_startPoint;
    QPoint m_endPoint;
    bool m_filled;
};

#endif // RECTANGLETOOL_H
