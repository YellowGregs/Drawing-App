#ifndef NEWDOCUMENTDIALOG_H
#define NEWDOCUMENTDIALOG_H

#include <QDialog>

class QSpinBox;
class QComboBox;
class QPushButton;

class NewDocumentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDocumentDialog(QWidget *parent = nullptr);
    
    int width() const;
    int height() const;
    QColor backgroundColor() const;

private slots:
    void onPresetChanged(int index);
    void onBackgroundColorClicked();

private:
    QSpinBox *m_widthSpinBox;
    QSpinBox *m_heightSpinBox;
    QComboBox *m_presetComboBox;
    QPushButton *m_bgColorButton;
    QColor m_backgroundColor;
};

#endif // NEWDOCUMENTDIALOG_H
