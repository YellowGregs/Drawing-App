#ifndef PENTOOL_H
#define PENTOOL_H

#include "tool.h"

class PenTool : public Tool
{
    Q_OBJECT

public:
    explicit PenTool(Canvas *canvas, QObject *parent = nullptr);
    
    void mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    
    QString getName() const override { return tr("Pen"); }

private:
    void drawLine(const QPoint &from, const QPoint &to);
};

#endif // PENTOOL_H
