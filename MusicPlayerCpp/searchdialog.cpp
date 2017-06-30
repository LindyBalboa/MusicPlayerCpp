#include "searchdialog.h"
#include "globals.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>

SearchDialog::SearchDialog(QWidget *parent, Qt::WindowFlags flags) :
    QDialog(parent, flags)
{
    anyEdit = new QLineEdit(this);
    titleEdit = new QLineEdit(this);
    artistEdit = new QLineEdit(this);
    genreEdit = new QLineEdit(this);
    commentEdit = new QLineEdit(this);

    QVBoxLayout *centralLayout = new QVBoxLayout(this);
    this->setLayout(centralLayout);

    anyOrEveryComboBox = new QComboBox(this);
    centralLayout->addWidget(anyOrEveryComboBox);
    anyOrEveryComboBox->addItem("Match Any");
    anyOrEveryComboBox->addItem("Match Each");
    QFormLayout *formLayout = new QFormLayout(this);
    centralLayout->addLayout(formLayout);
    formLayout->addRow("Any text field:", anyEdit);
    formLayout->addRow("Title:", titleEdit);
    formLayout->addRow("Artist:", artistEdit);
    formLayout->addRow("Genre:", genreEdit);
    formLayout->addRow("Comment:", commentEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout(this);
    centralLayout->addLayout(buttonLayout);
    QPushButton *okayButton = new QPushButton("Okay", this);
    okayButton->setFixedSize(okayButton->sizeHint());
    connect(okayButton, &QPushButton::clicked, this, &SearchDialog::okayClicked);
    buttonLayout->addWidget(okayButton, 0, Qt::AlignRight);
    QPushButton *cancelButton = new QPushButton("Cancel", this);
    buttonLayout->addWidget(cancelButton);
    cancelButton->setFixedSize(cancelButton->sizeHint());

}

void SearchDialog::okayClicked()
{

    qDebug() << "Search Dialog Okay button works";
    QString queryString = buildQuery();

    if (currentMode == SearchDialog::Search){
       emit newSearchQuery(queryString);
    } else if (currentMode == SearchDialog::NewSmartPlaylist){
       emit newSmartPlaylistQuery(queryString);
    }
}

QString SearchDialog::buildQuery()
{

    QString queryString;
    QStringList searchParams;
    if (!titleEdit->text().isEmpty())
        searchParams << "Title LIKE '" + titleEdit->text() + "'";
    if (!artistEdit->text().isEmpty())
        searchParams << "Artist LIKE '" + artistEdit->text() + "'";
    if (!genreEdit->text().isEmpty())
        searchParams << "Genre LIKE '" + genreEdit->text() + "'";
    if (!commentEdit->text().isEmpty())
        searchParams << "Comment LIKE '" + commentEdit->text() + "'";

    if (!anyEdit->text().isEmpty()){
        queryString.append("SELECT * FROM Search WHERE Search MATCH "  "'" + anyEdit->text() + "'"  " ");
        if (searchParams.isEmpty()){
            return queryString;
        }
        if (anyOrEveryComboBox->currentText()=="Match Any"){
            queryString.append("UNION SELECT * FROM Search WHERE ");
            queryString.append(searchParams.join(" OR "));
        }else if(anyOrEveryComboBox->currentText()=="Match Each"){
            queryString.append("AND ");
            queryString.append(searchParams.join(" AND "));
        }
    }else {
        queryString.append("SELECT * FROM Search ");
        if (searchParams.isEmpty()){
            return queryString;
        }
        queryString.append("WHERE ");
        if (anyOrEveryComboBox->currentText()=="Match Any"){
            queryString.append(searchParams.join(" OR "));
        }else if(anyOrEveryComboBox->currentText()=="Match Each"){
            queryString.append(searchParams.join(" AND "));
        }
    }
    return queryString;
}

void SearchDialog::openDialog(SearchDialog::Modes mode)
{
    currentMode = mode;

    if (currentMode == SearchDialog::Search){
        this->setWindowTitle("Search");
    } else if (currentMode == SearchDialog::NewSmartPlaylist){
        this->setWindowTitle("New Smart Playlist");
    }

    this->setVisible(true);
}
