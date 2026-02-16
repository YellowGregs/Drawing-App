#include "brushpreview.h"
#include <QPainter>

BrushPreview::BrushPreview(QWidget *parent)
    : QWidget(parent)
    , m_size(20)
    , m_hardness(0.5)
{
    setMinimumSize(100, 100);
}

void BrushPreview::setSize(int size)
{
    m_size = size;
    update();
}

void BrushPreview::setHardness(qreal hardness)
{
    m_hardness = hardness;
    update();
}

void BrushPreview::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    painter.fillRect(rect(), Qt::white);
    
    int centerX = width() / 2;
    int centerY = height() / 2;
    
    QRadialGradient gradient(centerX, centerY, m_size / 2);
    gradient.setColorAt(0, Qt::black);
    gradient.setColorAt(m_hardness, Qt::black);
    gradient.setColorAt(1, Qt::transparent);
    
    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPoint(centerX, centerY), m_size / 2, m_size / 2);
}
