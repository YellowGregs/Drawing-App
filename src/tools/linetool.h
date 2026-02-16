#ifndef LINETOOL_H
#define LINETOOL_H

#include "tool.h"

class LineTool : public Tool
{
    Q_OBJECT

public:
    explicit LineTool(Canvas *canvas, QObject *parent = nullptr);
    
    void mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void drawOverlay(QPainter &painter) override;
    
    QString getName() const override { return tr("Line"); }

private:
    QPoint m_startPoint;
    QPoint m_endPoint;
};

#endif // LINETOOL_H
