#ifndef SELECTTOOL_H
#define SELECTTOOL_H

#include "tool.h"
#include <QRect>

class SelectTool : public Tool
{
    Q_OBJECT

public:
    enum SelectMode {
        Rectangular,
        Elliptical,
        Lasso,
        MagicWand
    };

    explicit SelectTool(Canvas *canvas, QObject *parent = nullptr);
    
    void mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void drawOverlay(QPainter &painter) override;
    
    QString getName() const override { return tr("Select"); }
    
    void setSelectMode(SelectMode mode) { m_selectMode = mode; }
    SelectMode getSelectMode() const { return m_selectMode; }

private:
    QPoint m_startPoint;
    QPoint m_endPoint;
    SelectMode m_selectMode;
};

#endif // SELECTTOOL_H
