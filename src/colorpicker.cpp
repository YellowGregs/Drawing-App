#include "colorpicker.h"
#include "widgets/simplecolorpicker.h"
#include "widgets/colorpalette.h"
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>

ColorPicker::ColorPicker(QWidget *parent)
    : QWidget(parent)
    , m_currentColor(Qt::black)
    , m_simplePicker(nullptr)
    , m_colorPalette(nullptr)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    
    m_simplePicker = new SimpleColorPicker(this);
    m_simplePicker->setColor(m_currentColor);
    connect(m_simplePicker, &SimpleColorPicker::colorChanged, this, &ColorPicker::onColorChanged);
    layout->addWidget(m_simplePicker);
    
    QLabel *paletteLabel = new QLabel(tr("Presets:"), this);
    layout->addWidget(paletteLabel);
    
    QComboBox *paletteCombo = new QComboBox(this);
    paletteCombo->addItems({"Default", "Pastel", "Vibrant", "Earth Tones", "Grayscale"});
    layout->addWidget(paletteCombo);
    
    m_colorPalette = new ColorPalette(this);
    connect(m_colorPalette, &ColorPalette::colorSelected, this, &ColorPicker::setColor);
    connect(paletteCombo, &QComboBox::currentTextChanged, m_colorPalette, &ColorPalette::setPresetPalette);
    layout->addWidget(m_colorPalette);
    
    layout->addStretch();
}

void ColorPicker::setColor(const QColor &color)
{
    m_currentColor = color;
    if (m_simplePicker) {
        m_simplePicker->setColor(color);
    }
}

void ColorPicker::onColorChanged(const QColor &color)
{
    m_currentColor = color;
    emit colorChanged(color);
}
