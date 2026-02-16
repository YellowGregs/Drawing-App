#ifndef BRUSHTOOL_H
#define BRUSHTOOL_H

#include "tool.h"
#include <QImage>

class BrushTool : public Tool
{
    Q_OBJECT

public:
    explicit BrushTool(Canvas *canvas, QObject *parent = nullptr);
    ~BrushTool();

    void mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void drawOverlay(QPainter &painter) override;
    
    QString getName() const override { return tr("Brush"); }

private:
    void drawDab(const QPoint &pos, qreal pressure);
    void drawStroke(const QPoint &from, const QPoint &to, qreal pressure);
    QImage createBrushDab(int size, qreal hardness);
    
    QImage m_brushDab;
    QImage m_strokeBuffer;
    QImage m_beforeImage;  // For undo
};

#endif // BRUSHTOOL_H
