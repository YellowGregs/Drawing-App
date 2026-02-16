#ifndef PENCILTOOL_H
#define PENCILTOOL_H

#include "tool.h"

class PencilTool : public Tool
{
    Q_OBJECT

public:
    explicit PencilTool(Canvas *canvas, QObject *parent = nullptr);
    
    void mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    
    QString getName() const override { return tr("Pencil"); }

private:
    void drawPixel(const QPoint &pos);
    void drawLine(const QPoint &from, const QPoint &to);
};

#endif // PENCILTOOL_H
