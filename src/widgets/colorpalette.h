#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include <QWidget>
#include <QColor>
#include <QVector>

class QPushButton;
class QGridLayout;

class ColorPalette : public QWidget
{
    Q_OBJECT

public:
    explicit ColorPalette(QWidget *parent = nullptr);
    
    void addColor(const QColor &color);
    void setPresetPalette(const QString &paletteName);

signals:
    void colorSelected(const QColor &color);

private slots:
    void onColorButtonClicked();

private:
    void createDefaultPalette();
    void createPaletteButton(const QColor &color);
    
    QGridLayout *m_gridLayout;
    QVector<QColor> m_colors;
};

#endif // COLORPALETTE_H
