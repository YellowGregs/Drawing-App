#include "newdocumentdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QColorDialog>
#include <QDialogButtonBox>
#include <QLabel>

NewDocumentDialog::NewDocumentDialog(QWidget *parent)
    : QDialog(parent)
    , m_backgroundColor(Qt::white)
{
    setWindowTitle(tr("New Document"));
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Preset sizes
    m_presetComboBox = new QComboBox(this);
    m_presetComboBox->addItem(tr("Custom"));
    m_presetComboBox->addItem(tr("A4 (2480 x 3508)"));
    m_presetComboBox->addItem(tr("Letter (2550 x 3300)"));
    m_presetComboBox->addItem(tr("HD (1920 x 1080)"));
    m_presetComboBox->addItem(tr("4K (3840 x 2160)"));
    m_presetComboBox->addItem(tr("Square (1024 x 1024)"));
    m_presetComboBox->addItem(tr("Portrait (800 x 1200)"));
    m_presetComboBox->addItem(tr("Landscape (1200 x 800)"));
    connect(m_presetComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NewDocumentDialog::onPresetChanged);
    
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow(tr("Preset:"), m_presetComboBox);
    
    // Width and height
    m_widthSpinBox = new QSpinBox(this);
    m_widthSpinBox->setRange(1, 10000);
    m_widthSpinBox->setValue(800);
    formLayout->addRow(tr("Width:"), m_widthSpinBox);
    
    m_heightSpinBox = new QSpinBox(this);
    m_heightSpinBox->setRange(1, 10000);
    m_heightSpinBox->setValue(600);
    formLayout->addRow(tr("Height:"), m_heightSpinBox);
    
    // Background color
    QHBoxLayout *bgLayout = new QHBoxLayout();
    m_bgColorButton = new QPushButton(this);
    m_bgColorButton->setFixedSize(50, 25);
    m_bgColorButton->setStyleSheet(QString("background-color: %1; border: 1px solid black;")
                                  .arg(m_backgroundColor.name()));
    connect(m_bgColorButton, &QPushButton::clicked, this, &NewDocumentDialog::onBackgroundColorClicked);
    bgLayout->addWidget(m_bgColorButton);
    bgLayout->addWidget(new QLabel(tr("Background Color")));
    bgLayout->addStretch();
    formLayout->addRow(tr(""), bgLayout);
    
    mainLayout->addLayout(formLayout);
    
    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);
}

int NewDocumentDialog::width() const
{
    return m_widthSpinBox->value();
}

int NewDocumentDialog::height() const
{
    return m_heightSpinBox->value();
}

QColor NewDocumentDialog::backgroundColor() const
{
    return m_backgroundColor;
}

void NewDocumentDialog::onPresetChanged(int index)
{
    switch (index) {
        case 1: // A4
            m_widthSpinBox->setValue(2480);
            m_heightSpinBox->setValue(3508);
            break;
        case 2: // Letter
            m_widthSpinBox->setValue(2550);
            m_heightSpinBox->setValue(3300);
            break;
        case 3: // HD
            m_widthSpinBox->setValue(1920);
            m_heightSpinBox->setValue(1080);
            break;
        case 4: // 4K
            m_widthSpinBox->setValue(3840);
            m_heightSpinBox->setValue(2160);
            break;
        case 5: // Square
            m_widthSpinBox->setValue(1024);
            m_heightSpinBox->setValue(1024);
            break;
        case 6: // Portrait
            m_widthSpinBox->setValue(800);
            m_heightSpinBox->setValue(1200);
            break;
        case 7: // Landscape
            m_widthSpinBox->setValue(1200);
            m_heightSpinBox->setValue(800);
            break;
        default:
            break;
    }
}

void NewDocumentDialog::onBackgroundColorClicked()
{
    QColor color = QColorDialog::getColor(m_backgroundColor, this, tr("Select Background Color"));
    if (color.isValid()) {
        m_backgroundColor = color;
        m_bgColorButton->setStyleSheet(QString("background-color: %1; border: 1px solid black;")
                                      .arg(m_backgroundColor.name()));
    }
}
