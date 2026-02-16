#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include <QColor>

class SimpleColorPicker;
class ColorPalette;

class ColorPicker : public QWidget
{
    Q_OBJECT

public:
    explicit ColorPicker(QWidget *parent = nullptr);
    
    QColor getColor() const { return m_currentColor; }
    void setColor(const QColor &color);

signals:
    void colorChanged(const QColor &color);

private slots:
    void onColorChanged(const QColor &color);

private:
    QColor m_currentColor;
    SimpleColorPicker *m_simplePicker;
    ColorPalette *m_colorPalette;
};

#endif // COLORPICKER_H
