#ifndef MODELS_H
#define MODELS_H

#include <QItemSelectionModel>
#include <QPoint>
#include <QTableView>
#include <Qt>
#include <QtSql>
#include <QTreeView>

class LibrarySqlTableModel;
class LibraryTable : public QTableView
{
    Q_OBJECT

    public:
        LibraryTable(QSqlDatabase database, QWidget *parent =0);
        ~LibraryTable();
        void changeView(QString oldView, QString newView);

        QSqlDatabase libraryDb;
        QSqlQuery query;
protected:
        void keyReleaseEvent(QKeyEvent *event);
        LibrarySqlTableModel *libraryModel;
private:
       QPoint startDragPos;
       void mousePressEvent(QMouseEvent *event); Q_DECL_OVERRIDE
       void mouseMoveEvent(QMouseEvent *event); Q_DECL_OVERRIDE
};

class LibraryTree : public QTreeView
{
    Q_OBJECT
public:
    LibraryTree(QSqlDatabase &database, QWidget *parent=0);
    ~LibraryTree();
signals:
    newViewClicked(QString oldView, QString newView);
    requestOptions(QString option);
private:
    QString currentView;
protected:
    QSqlDatabase libraryDb;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void contextMenuEvent(QContextMenuEvent *);
    void emitRequestOptions();
};

class LibrarySqlTableModel : public QSqlTableModel
{
    Q_OBJECT

public:
    LibrarySqlTableModel(QWidget *parent = 0, QSqlDatabase database = QSqlDatabase());
    ~LibrarySqlTableModel();

    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setSelectStatement(QString queryString);
protected:
    QString selectStatementString;
    QString selectStatement() const Q_DECL_OVERRIDE;
private:
    QItemSelectionModel *_selectionModel;
    QMimeData* mimeData(const QModelIndexList &indexes) const Q_DECL_OVERRIDE;
    Qt::DropActions supportedDragActions() const Q_DECL_OVERRIDE;
};

#endif // MODELS_H
