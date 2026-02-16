#include "colorwheel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>

ColorWheel::ColorWheel(QWidget *parent)
    : QWidget(parent)
    , m_currentColor(Qt::red)
    , m_wheelWidth(20)
{
    setMinimumSize(100, 100);
}

void ColorWheel::setColor(const QColor &color)
{
    m_currentColor = color;
    update();
}

void ColorWheel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    if (m_wheelImage.isNull() || m_wheelImage.size() != size()) {
        generateWheel();
    }
    
    painter.drawImage(0, 0, m_wheelImage);
    
    int centerX = width() / 2;
    int centerY = height() / 2;
    int radius = qMin(width(), height()) / 2 - m_wheelWidth;
    
    int hue = m_currentColor.hue();
    int sat = m_currentColor.saturation();
    int val = m_currentColor.value();
    
    qreal angle = hue * M_PI / 180.0;
    qreal dist = (sat / 255.0) * radius;
    
    int x = centerX + dist * qCos(angle);
    int y = centerY + dist * qSin(angle);
    
    painter.setPen(QPen(Qt::white, 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(QPoint(x, y), 5, 5);
    painter.setPen(QPen(Qt::black, 1));
    painter.drawEllipse(QPoint(x, y), 6, 6);
}

void ColorWheel::mousePressEvent(QMouseEvent *event)
{
    updateColorFromPosition(event->pos());
}

void ColorWheel::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        updateColorFromPosition(event->pos());
    }
}

void ColorWheel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    generateWheel();
}

void ColorWheel::updateColorFromPosition(const QPoint &pos)
{
    QColor color = getColorAt(pos);
    if (color.isValid()) {
        m_currentColor = color;
        m_currentColor.setAlpha(255);
        update();
        emit colorChanged(m_currentColor);
    }
}

void ColorWheel::generateWheel()
{
    int size = qMin(width(), height());
    m_wheelImage = QImage(width(), height(), QImage::Format_ARGB32);
    m_wheelImage.fill(Qt::transparent);
    
    int centerX = width() / 2;
    int centerY = height() / 2;
    int outerRadius = size / 2;
    int innerRadius = outerRadius - m_wheelWidth;
    
    for (int y = 0; y < height(); ++y) {
        for (int x = 0; x < width(); ++x) {
            int dx = x - centerX;
            int dy = y - centerY;
            qreal dist = qSqrt(dx * dx + dy * dy);
            
            if (dist >= innerRadius && dist <= outerRadius) {
                // Hue ring
                qreal angle = qAtan2(dy, dx);
                int hue = (int)((angle * 180.0 / M_PI) + 360) % 360;
                QColor color = QColor::fromHsv(hue, 255, 255);
                m_wheelImage.setPixelColor(x, y, color);
            } else if (dist < innerRadius) {
                // Saturation/Value square
                qreal saturation = dist / innerRadius;
                int hue = m_currentColor.hue();
                QColor color = QColor::fromHsv(hue, saturation * 255, 255);
                m_wheelImage.setPixelColor(x, y, color);
            }
        }
    }
}

QColor ColorWheel::getColorAt(const QPoint &pos)
{
    int centerX = width() / 2;
    int centerY = height() / 2;
    int dx = pos.x() - centerX;
    int dy = pos.y() - centerY;
    qreal dist = qSqrt(dx * dx + dy * dy);
    
    int outerRadius = qMin(width(), height()) / 2;
    int innerRadius = outerRadius - m_wheelWidth;
    
    if (dist >= innerRadius && dist <= outerRadius) {
        // Hue ring
        qreal angle = qAtan2(dy, dx);
        int hue = (int)((angle * 180.0 / M_PI) + 360) % 360;
        return QColor::fromHsv(hue, 255, 255);
    } else if (dist < innerRadius) {
        // Saturation area
        qreal saturation = dist / innerRadius;
        int hue = m_currentColor.hue();
        return QColor::fromHsv(hue, saturation * 255, 255);
    }
    
    return QColor();
}
