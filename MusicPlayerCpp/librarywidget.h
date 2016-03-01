#ifndef MODELS_H
#define MODELS_H
#include <QtSql>
#include <Qt>
#include <QTableView>

class LibraryWidget : public QTableView
{
    Q_OBJECT

    public:
        LibraryWidget(QWidget *parent =0);
        ~LibraryWidget();

};

class LibrarySqlTableModel : public QSqlTableModel
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
