#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include <QWidget>
#include <QColor>

class ColorWheel : public QWidget
{
    Q_OBJECT

public:
    explicit ColorWheel(QWidget *parent = nullptr);
    
    QColor getColor() const { return m_currentColor; }
    void setColor(const QColor &color);

signals:
    void colorChanged(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateColorFromPosition(const QPoint &pos);
    void generateWheel();
    QColor getColorAt(const QPoint &pos);
    
    QColor m_currentColor;
    QImage m_wheelImage;
    int m_wheelWidth;
};

#endif // COLORWHEEL_H
