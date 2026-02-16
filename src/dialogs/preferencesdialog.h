#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

class QSpinBox;
class QCheckBox;
class QComboBox;

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);

private slots:
    void saveSettings();

private:
    void loadSettings();
    
    QSpinBox *m_undoLimitSpinBox;
    QCheckBox *m_autoSaveCheckBox;
    QSpinBox *m_autoSaveIntervalSpinBox;
    QCheckBox *m_smoothingCheckBox;
    QComboBox *m_themeComboBox;
};

#endif // PREFERENCESDIALOG_H
