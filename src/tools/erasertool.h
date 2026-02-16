#ifndef ERASERTOOL_H
#define ERASERTOOL_H

#include "tool.h"

class EraserTool : public Tool
{
    Q_OBJECT

public:
    explicit EraserTool(Canvas *canvas, QObject *parent = nullptr);
    
    void mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    
    QString getName() const override { return tr("Eraser"); }

private:
    void erase(const QPoint &pos, qreal pressure);
    void eraseStroke(const QPoint &from, const QPoint &to, qreal pressure);
};

#endif // ERASERTOOL_H
