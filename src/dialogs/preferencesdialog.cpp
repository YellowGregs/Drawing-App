#include "preferencesdialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QSettings>

PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Preferences"));
    setMinimumWidth(400);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // General settings
    QGroupBox *generalGroup = new QGroupBox(tr("General"), this);
    QFormLayout *generalLayout = new QFormLayout(generalGroup);
    
    m_undoLimitSpinBox = new QSpinBox(this);
    m_undoLimitSpinBox->setRange(10, 500);
    m_undoLimitSpinBox->setValue(100);
    generalLayout->addRow(tr("Undo Limit:"), m_undoLimitSpinBox);
    
    m_themeComboBox = new QComboBox(this);
    m_themeComboBox->addItems({"Light", "Dark", "System"});
    generalLayout->addRow(tr("Theme:"), m_themeComboBox);
    
    mainLayout->addWidget(generalGroup);
    
    // Auto-save settings
    QGroupBox *autoSaveGroup = new QGroupBox(tr("Auto-Save"), this);
    QFormLayout *autoSaveLayout = new QFormLayout(autoSaveGroup);
    
    m_autoSaveCheckBox = new QCheckBox(tr("Enable Auto-Save"), this);
    autoSaveLayout->addRow(m_autoSaveCheckBox);
    
    m_autoSaveIntervalSpinBox = new QSpinBox(this);
    m_autoSaveIntervalSpinBox->setRange(1, 60);
    m_autoSaveIntervalSpinBox->setValue(5);
    m_autoSaveIntervalSpinBox->setSuffix(tr(" minutes"));
    autoSaveLayout->addRow(tr("Interval:"), m_autoSaveIntervalSpinBox);
    
    mainLayout->addWidget(autoSaveGroup);
    
    // Drawing settings
    QGroupBox *drawingGroup = new QGroupBox(tr("Drawing"), this);
    QFormLayout *drawingLayout = new QFormLayout(drawingGroup);
    
    m_smoothingCheckBox = new QCheckBox(tr("Enable Brush Smoothing"), this);
    m_smoothingCheckBox->setChecked(true);
    drawingLayout->addRow(m_smoothingCheckBox);
    
    mainLayout->addWidget(drawingGroup);
    
    mainLayout->addStretch();
    
    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &PreferencesDialog::saveSettings);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);
    
    loadSettings();
}

void PreferencesDialog::saveSettings()
{
    QSettings settings("DrawingApp", "ProfessionalDrawingApp");
    
    settings.setValue("undoLimit", m_undoLimitSpinBox->value());
    settings.setValue("autoSave", m_autoSaveCheckBox->isChecked());
    settings.setValue("autoSaveInterval", m_autoSaveIntervalSpinBox->value());
    settings.setValue("brushSmoothing", m_smoothingCheckBox->isChecked());
    settings.setValue("theme", m_themeComboBox->currentText());
    
    accept();
}

void PreferencesDialog::loadSettings()
{
    QSettings settings("DrawingApp", "ProfessionalDrawingApp");
    
    m_undoLimitSpinBox->setValue(settings.value("undoLimit", 100).toInt());
    m_autoSaveCheckBox->setChecked(settings.value("autoSave", false).toBool());
    m_autoSaveIntervalSpinBox->setValue(settings.value("autoSaveInterval", 5).toInt());
    m_smoothingCheckBox->setChecked(settings.value("brushSmoothing", true).toBool());
    
    QString theme = settings.value("theme", "Light").toString();
    m_themeComboBox->setCurrentText(theme);
}
