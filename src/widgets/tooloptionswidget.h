#ifndef TOOLOPTIONSWIDGET_H
#define TOOLOPTIONSWIDGET_H

#include <QWidget>

class Tool;
class QSlider;
class QSpinBox;
class QCheckBox;
class BrushPreview;

class ToolOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToolOptionsWidget(QWidget *parent = nullptr);
    
    void setTool(Tool *tool);

private slots:
    void onSizeChanged(int value);
    void onOpacityChanged(int value);
    void onHardnessChanged(int value);

private:
    void updateFromTool();
    
    Tool *m_currentTool;
    QSpinBox *m_sizeSpinBox;
    QSlider *m_opacitySlider;
    QSlider *m_hardnessSlider;
    BrushPreview *m_brushPreview;
};

#endif // TOOLOPTIONSWIDGET_H
