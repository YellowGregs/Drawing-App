#ifndef POLYGONTOOL_H
#define POLYGONTOOL_H

#include "tool.h"
#include <QVector>

class PolygonTool : public Tool
{
    Q_OBJECT

public:
    explicit PolygonTool(Canvas *canvas, QObject *parent = nullptr);
    
    void mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void drawOverlay(QPainter &painter) override;
    
    QString getName() const override { return tr("Polygon"); }
    
    void setFilled(bool filled) { m_filled = filled; }
    bool isFilled() const { return m_filled; }

private:
    QVector<QPoint> m_points;
    QPoint m_currentPoint;
    bool m_filled;
};

#endif // POLYGONTOOL_H
