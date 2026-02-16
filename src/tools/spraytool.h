#ifndef SPRAYTOOL_H
#define SPRAYTOOL_H

#include "tool.h"
#include <QImage>
#include <QTimer>

class SprayTool : public Tool
{
    Q_OBJECT

public:
    explicit SprayTool(Canvas *canvas, QObject *parent = nullptr);
    ~SprayTool();

    void mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void drawOverlay(QPainter &painter) override;
    
    QString getName() const override { return tr("Spray"); }
    
    void setDensity(qreal density) { m_density = qBound(0.1, density, 1.0); }
    qreal getDensity() const { return m_density; }

private slots:
    void sprayPaint();

private:
    void spraySinglePoint(const QPoint &pos, qreal pressure);
    
    QImage m_strokeBuffer;
    QImage m_beforeImage;
    QPoint m_currentPos;
    qreal m_density;
    QTimer *m_sprayTimer;
    qreal m_currentPressure;
};

#endif // SPRAYTOOL_H
