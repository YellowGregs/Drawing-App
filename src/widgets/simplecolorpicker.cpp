#include "simplecolorpicker.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>
#include <QColorDialog>

SimpleColorPicker::SimpleColorPicker(QWidget *parent)
    : QWidget(parent)
    , current_color(Qt::black)
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->setSpacing(8);
    main_layout->setContentsMargins(8, 8, 8, 8);
    
    // Color preview
    color_preview = new QLabel(this);
    color_preview->setMinimumHeight(60);
    color_preview->setFrameStyle(QFrame::Box | QFrame::Sunken);
    color_preview->setStyleSheet("background-color: black; border: 2px solid gray;");
    main_layout->addWidget(color_preview);
    
    // Color picker button
    choose_button = new QPushButton("Choose Color...", this);
    connect(choose_button, &QPushButton::clicked, this, &SimpleColorPicker::onChooseColorClicked);
    main_layout->addWidget(choose_button);
    
    // RGB sliders
    QGridLayout *slider_layout = new QGridLayout();
    slider_layout->setSpacing(6);
    
    // Red
    QLabel *red_label = new QLabel("Red:", this);
    red_slider = new QSlider(Qt::Horizontal, this);
    red_slider->setRange(0, 255);
    red_slider->setValue(0);
    red_spin = new QSpinBox(this);
    red_spin->setRange(0, 255);
    red_spin->setValue(0);
    
    slider_layout->addWidget(red_label, 0, 0);
    slider_layout->addWidget(red_slider, 0, 1);
    slider_layout->addWidget(red_spin, 0, 2);
    
    // Green
    QLabel *green_label = new QLabel("Green:", this);
    green_slider = new QSlider(Qt::Horizontal, this);
    green_slider->setRange(0, 255);
    green_slider->setValue(0);
    green_spin = new QSpinBox(this);
    green_spin->setRange(0, 255);
    green_spin->setValue(0);
    
    slider_layout->addWidget(green_label, 1, 0);
    slider_layout->addWidget(green_slider, 1, 1);
    slider_layout->addWidget(green_spin, 1, 2);
    
    // Blue
    QLabel *blue_label = new QLabel("Blue:", this);
    blue_slider = new QSlider(Qt::Horizontal, this);
    blue_slider->setRange(0, 255);
    blue_slider->setValue(0);
    blue_spin = new QSpinBox(this);
    blue_spin->setRange(0, 255);
    blue_spin->setValue(0);
    
    slider_layout->addWidget(blue_label, 2, 0);
    slider_layout->addWidget(blue_slider, 2, 1);
    slider_layout->addWidget(blue_spin, 2, 2);
    
    main_layout->addLayout(slider_layout);
    main_layout->addStretch();
    
    // Connect signals
    connect(red_slider, &QSlider::valueChanged, red_spin, &QSpinBox::setValue);
    connect(red_spin, QOverload<int>::of(&QSpinBox::valueChanged), red_slider, &QSlider::setValue);
    connect(red_slider, &QSlider::valueChanged, this, &SimpleColorPicker::onRedChanged);
    
    connect(green_slider, &QSlider::valueChanged, green_spin, &QSpinBox::setValue);
    connect(green_spin, QOverload<int>::of(&QSpinBox::valueChanged), green_slider, &QSlider::setValue);
    connect(green_slider, &QSlider::valueChanged, this, &SimpleColorPicker::onGreenChanged);
    
    connect(blue_slider, &QSlider::valueChanged, blue_spin, &QSpinBox::setValue);
    connect(blue_spin, QOverload<int>::of(&QSpinBox::valueChanged), blue_slider, &QSlider::setValue);
    connect(blue_slider, &QSlider::valueChanged, this, &SimpleColorPicker::onBlueChanged);
    
    updateColorDisplay();
}

void SimpleColorPicker::setColor(const QColor &color)
{
    current_color = color;
    
    red_slider->blockSignals(true);
    green_slider->blockSignals(true);
    blue_slider->blockSignals(true);
    
    red_slider->setValue(color.red());
    green_slider->setValue(color.green());
    blue_slider->setValue(color.blue());
    
    red_slider->blockSignals(false);
    green_slider->blockSignals(false);
    blue_slider->blockSignals(false);
    
    updateColorDisplay();
}

void SimpleColorPicker::onRedChanged(int value)
{
    current_color.setRed(value);
    updateColorDisplay();
    emit colorChanged(current_color);
}

void SimpleColorPicker::onGreenChanged(int value)
{
    current_color.setGreen(value);
    updateColorDisplay();
    emit colorChanged(current_color);
}

void SimpleColorPicker::onBlueChanged(int value)
{
    current_color.setBlue(value);
    updateColorDisplay();
    emit colorChanged(current_color);
}

void SimpleColorPicker::onChooseColorClicked()
{
    QColor color = QColorDialog::getColor(current_color, this, "Choose Color");
    if (color.isValid()) {
        setColor(color);
        emit colorChanged(current_color);
    }
}

void SimpleColorPicker::updateColorDisplay()
{
    color_preview->setStyleSheet(QString("background-color: rgb(%1, %2, %3); border: 2px solid gray;")
        .arg(current_color.red())
        .arg(current_color.green())
        .arg(current_color.blue()));
}
