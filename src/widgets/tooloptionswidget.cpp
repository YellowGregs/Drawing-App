#include "tooloptionswidget.h"
#include "brushpreview.h"
#include "../tools/tool.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QGroupBox>

ToolOptionsWidget::ToolOptionsWidget(QWidget *parent)
    : QWidget(parent)
    , m_currentTool(nullptr)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Brush preview
    m_brushPreview = new BrushPreview(this);
    mainLayout->addWidget(m_brushPreview);
    
    // Size control
    QHBoxLayout *sizeLayout = new QHBoxLayout();
    sizeLayout->addWidget(new QLabel(tr("Size:")));
    m_sizeSpinBox = new QSpinBox(this);
    m_sizeSpinBox->setRange(1, 500);
    m_sizeSpinBox->setValue(20);
    connect(m_sizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            this, &ToolOptionsWidget::onSizeChanged);
    sizeLayout->addWidget(m_sizeSpinBox);
    mainLayout->addLayout(sizeLayout);
    
    // Opacity control
    QHBoxLayout *opacityLayout = new QHBoxLayout();
    opacityLayout->addWidget(new QLabel(tr("Opacity:")));
    m_opacitySlider = new QSlider(Qt::Horizontal, this);
    m_opacitySlider->setRange(0, 100);
    m_opacitySlider->setValue(100);
    connect(m_opacitySlider, &QSlider::valueChanged, 
            this, &ToolOptionsWidget::onOpacityChanged);
    opacityLayout->addWidget(m_opacitySlider);
    mainLayout->addLayout(opacityLayout);
    
    // Hardness control
    QHBoxLayout *hardnessLayout = new QHBoxLayout();
    hardnessLayout->addWidget(new QLabel(tr("Hardness:")));
    m_hardnessSlider = new QSlider(Qt::Horizontal, this);
    m_hardnessSlider->setRange(0, 100);
    m_hardnessSlider->setValue(50);
    connect(m_hardnessSlider, &QSlider::valueChanged, 
            this, &ToolOptionsWidget::onHardnessChanged);
    hardnessLayout->addWidget(m_hardnessSlider);
    mainLayout->addLayout(hardnessLayout);
    
    mainLayout->addStretch();
}

void ToolOptionsWidget::setTool(Tool *tool)
{
    m_currentTool = tool;
    updateFromTool();
}

void ToolOptionsWidget::onSizeChanged(int value)
{
    if (m_currentTool) {
        m_currentTool->setSize(value);
        m_brushPreview->setSize(value);
    }
}

void ToolOptionsWidget::onOpacityChanged(int value)
{
    if (m_currentTool) {
        m_currentTool->setOpacity(value / 100.0);
    }
}

void ToolOptionsWidget::onHardnessChanged(int value)
{
    if (m_currentTool) {
        m_currentTool->setHardness(value / 100.0);
        m_brushPreview->setHardness(value / 100.0);
    }
}

void ToolOptionsWidget::updateFromTool()
{
    if (!m_currentTool) return;
    
    m_sizeSpinBox->blockSignals(true);
    m_opacitySlider->blockSignals(true);
    m_hardnessSlider->blockSignals(true);
    
    m_sizeSpinBox->setValue(m_currentTool->getSize());
    m_opacitySlider->setValue(m_currentTool->getOpacity() * 100);
    m_hardnessSlider->setValue(m_currentTool->getHardness() * 100);
    
    m_brushPreview->setSize(m_currentTool->getSize());
    m_brushPreview->setHardness(m_currentTool->getHardness());
    
    m_sizeSpinBox->blockSignals(false);
    m_opacitySlider->blockSignals(false);
    m_hardnessSlider->blockSignals(false);
}
