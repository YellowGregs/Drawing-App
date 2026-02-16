#include "aboutdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("About Drawing App"));
    setMinimumWidth(400);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    QLabel *titleLabel = new QLabel(tr("<h1>Drawing App</h1>"), this);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    QLabel *versionLabel = new QLabel(tr("<p>Version 1.0.0</p>"), this);
    versionLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(versionLabel);
    
    QLabel *descLabel = new QLabel(
        tr("<p>A drawing application built with Qt and C++.</p>"
           "</ul>"), this);
    descLabel->setWordWrap(true);
    mainLayout->addWidget(descLabel);
    
    QLabel *copyrightLabel = new QLabel(tr("<p>© 2026 Drawing App Team</p>"), this);
    copyrightLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(copyrightLabel);
    
    mainLayout->addStretch();
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    mainLayout->addWidget(buttonBox);
}
