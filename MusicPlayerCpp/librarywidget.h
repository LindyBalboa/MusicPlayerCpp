#ifndef MODELS_H
#define MODELS_H
#include <QtSql>
#include <Qt>
#include <QTableView>

class LibraryWidget : public QTableView
{
    Q_OBJECT

    public:
        LibraryWidget(QSqlDatabase database, QWidget *parent =0);
        ~LibraryWidget();
        QSqlDatabase libraryDb;
};

class LibrarySqlTableModel : public QSqlQueryModel
{
    Q_OBJECT

    public:
        LibrarySqlTableModel(QWidget *parent = 0, QSqlDatabase database = QSqlDatabase());
        ~LibrarySqlTableModel();
        Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    private:
        Qt::DropActions supportedDragActions() const Q_DECL_OVERRIDE;
        QMimeData* mimeData(const QModelIndexList &indexes) const Q_DECL_OVERRIDE;
};

#endif // MODELS_H
