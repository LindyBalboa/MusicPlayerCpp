#include "searchdialog.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLineEdit>

SearchDialog::SearchDialog(QWidget *parent, Qt::WindowFlags flags) : QWidget(parent, flags)
{
    QLineEdit *anyEdit = new QLineEdit(this);
    QLineEdit *artistEdit = new QLineEdit(this);
    QLineEdit *genreEdit = new QLineEdit(this);
    QLineEdit *commentEdit = new QLineEdit(this);

    QVBoxLayout *centralLayout = new QVBoxLayout(this);
    this->setLayout(centralLayout);
    QFormLayout *formLayout = new QFormLayout(this);
    centralLayout->addLayout(formLayout);
    formLayout->addRow("Any text field", anyEdit);
    formLayout->addRow("Artist", artistEdit);
    formLayout->addRow("Genre", genreEdit);
    formLayout->addRow("Comments", commentEdit);

}
