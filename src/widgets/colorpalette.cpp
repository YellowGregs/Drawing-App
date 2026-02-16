#include "colorpalette.h"
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

ColorPalette::ColorPalette(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    
    QLabel *label = new QLabel(tr("Color Palette"), this);
    mainLayout->addWidget(label);
    
    m_gridLayout = new QGridLayout();
    m_gridLayout->setSpacing(2);
    mainLayout->addLayout(m_gridLayout);
    
    mainLayout->addStretch();
    
    createDefaultPalette();
}

void ColorPalette::addColor(const QColor &color)
{
    m_colors.append(color);
    createPaletteButton(color);
}

void ColorPalette::setPresetPalette(const QString &paletteName)
{
    QLayoutItem *item;
    while ((item = m_gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    m_colors.clear();
    
    if (paletteName == "Default") {
        createDefaultPalette();
    } else if (paletteName == "Pastel") {
        // Pastel colors
        addColor(QColor(255, 179, 186)); // Pink
        addColor(QColor(255, 223, 186)); // Peach
        addColor(QColor(255, 255, 186)); // Yellow
        addColor(QColor(186, 255, 201)); // Mint
        addColor(QColor(186, 225, 255)); // Sky blue
        addColor(QColor(216, 191, 216)); // Lavender
        addColor(QColor(255, 200, 221)); // Rose
        addColor(QColor(221, 221, 221)); // Light gray
    } else if (paletteName == "Vibrant") {
        // Vibrant colors
        addColor(QColor(255, 0, 0));     // Red
        addColor(QColor(255, 127, 0));   // Orange
        addColor(QColor(255, 255, 0));   // Yellow
        addColor(QColor(0, 255, 0));     // Green
        addColor(QColor(0, 127, 255));   // Blue
        addColor(QColor(75, 0, 130));    // Indigo
        addColor(QColor(148, 0, 211));   // Violet
        addColor(QColor(255, 0, 255));   // Magenta
    } else if (paletteName == "Earth Tones") {
        // Earth tones
        addColor(QColor(139, 90, 43));   // Brown
        addColor(QColor(160, 120, 90));  // Tan
        addColor(QColor(194, 178, 128)); // Sand
        addColor(QColor(107, 142, 35));  // Olive
        addColor(QColor(85, 107, 47));   // Dark olive
        addColor(QColor(139, 69, 19));   // Saddle brown
        addColor(QColor(210, 180, 140)); // Tan
        addColor(QColor(188, 143, 143)); // Rosy brown
    } else if (paletteName == "Grayscale") {
        // Grayscale
        addColor(QColor(0, 0, 0));       // Black
        addColor(QColor(32, 32, 32));
        addColor(QColor(64, 64, 64));
        addColor(QColor(96, 96, 96));
        addColor(QColor(128, 128, 128));
        addColor(QColor(160, 160, 160));
        addColor(QColor(192, 192, 192));
        addColor(QColor(224, 224, 224));
        addColor(QColor(255, 255, 255)); // White
    }
}

void ColorPalette::onColorButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColor color = button->property("color").value<QColor>();
        emit colorSelected(color);
    }
}

void ColorPalette::createDefaultPalette()
{
    QVector<QColor> defaultColors = {
        Qt::black, Qt::white, Qt::red, Qt::green,
        Qt::blue, Qt::yellow, Qt::cyan, Qt::magenta,
        QColor(128, 0, 0),    // Maroon
        QColor(128, 128, 0),  // Olive
        QColor(0, 128, 0),    // Dark green
        QColor(128, 0, 128),  // Purple
        QColor(0, 128, 128),  // Teal
        QColor(0, 0, 128),    // Navy
        QColor(255, 165, 0),  // Orange
        QColor(255, 192, 203) // Pink
    };
    
    for (const QColor &color : defaultColors) {
        addColor(color);
    }
}

void ColorPalette::createPaletteButton(const QColor &color)
{
    QPushButton *button = new QPushButton(this);
    button->setFixedSize(24, 24);
    button->setStyleSheet(QString("background-color: %1; border: 1px solid #888;").arg(color.name()));
    button->setProperty("color", color);
    connect(button, &QPushButton::clicked, this, &ColorPalette::onColorButtonClicked);
    
    int count = m_gridLayout->count();
    int row = count / 4;
    int col = count % 4;
    m_gridLayout->addWidget(button, row, col);
}
