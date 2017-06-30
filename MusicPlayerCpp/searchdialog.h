#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = 0, Qt::WindowFlags flags=Qt::Window);

    enum Modes {Search,
                NewSmartPlaylist
                 };
    Q_ENUM(Modes);

    void openDialog(SearchDialog::Modes mode);

signals:
    QString newSearchQuery(QString query);
    QString newSmartPlaylistQuery(QString query);

private:
    QString buildQuery();
    QLineEdit *anyEdit;
    QLineEdit *titleEdit;
    QLineEdit *artistEdit;
    QLineEdit *genreEdit;
    QLineEdit *commentEdit;
    QComboBox *anyOrEveryComboBox;
    SearchDialog::Modes currentMode;
    void okayClicked();
};

#endif // SEARCHDIALOG_H
