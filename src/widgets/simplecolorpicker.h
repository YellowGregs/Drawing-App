#ifndef SIMPLECOLORPICKER_H
#define SIMPLECOLORPICKER_H

#include <QWidget>
#include <QColor>
#include <QFrame>

class QSlider;
class QSpinBox;
class QPushButton;
class QLabel;

class SimpleColorPicker : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleColorPicker(QWidget *parent = nullptr);
    
    QColor getColor() const { return current_color; }
    void setColor(const QColor &color);

signals:
    void colorChanged(const QColor &color);

private slots:
    void onRedChanged(int value);
    void onGreenChanged(int value);
    void onBlueChanged(int value);
    void onChooseColorClicked();
    void updateColorDisplay();

private:
    QColor current_color;
    
    QLabel *color_preview;
    QSlider *red_slider;
    QSlider *green_slider;
    QSlider *blue_slider;
    QSpinBox *red_spin;
    QSpinBox *green_spin;
    QSpinBox *blue_spin;
    QPushButton *choose_button;
};

#endif // SIMPLECOLORPICKER_H
