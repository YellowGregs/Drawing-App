#ifndef BRUSHPREVIEW_H
#define BRUSHPREVIEW_H

#include <QWidget>

class BrushPreview : public QWidget
{
    Q_OBJECT

public:
    explicit BrushPreview(QWidget *parent = nullptr);
    
    void setSize(int size);
    void setHardness(qreal hardness);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_size;
    qreal m_hardness;
};

#endif // BRUSHPREVIEW_H
