#ifndef FILLTOOL_H
#define FILLTOOL_H

#include "tool.h"

class FillTool : public Tool
{
    Q_OBJECT

public:
    explicit FillTool(Canvas *canvas, QObject *parent = nullptr);
    
    void mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    
    QString getName() const override { return tr("Fill"); }
    
    void setTolerance(int tolerance) { m_tolerance = tolerance; }
    int getTolerance() const { return m_tolerance; }

private:
    void floodFill(QImage *image, const QPoint &pos, const QColor &fillColor, const QColor &targetColor);
    bool colorMatch(const QColor &c1, const QColor &c2, int tolerance);
    
    int m_tolerance;
};

#endif // FILLTOOL_H
