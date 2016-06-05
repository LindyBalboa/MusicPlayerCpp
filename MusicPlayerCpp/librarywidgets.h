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
        void changeView(QString newView);
        void reselectUpdate();

        QSqlDatabase libraryDb;
        QSqlQuery query;
public slots:
        void saveColumnOrder(QString viewName);
protected:
        void keyReleaseEvent(QKeyEvent *event);
        LibrarySqlTableModel *libraryModel;
private:
       QPoint startDragPos;
       void mousePressEvent(QMouseEvent *event);
       void mouseMoveEvent(QMouseEvent *event);
};

class LibraryTree : public QTreeView
{
    Q_OBJECT
public:
    LibraryTree(QSqlDatabase &database, QWidget *parent=0);
    ~LibraryTree();
    QString currentView;
    void populateTree();
signals:
    newViewClicked(QString newView);
    requestOptions(QString option);
private:
protected:
    QSqlDatabase libraryDb;
        QSqlQuery query;
    void mouseReleaseEvent(QMouseEvent *event) ;
    void contextMenuEvent(QContextMenuEvent *);
    void emitRequestOptions();
};

class LibrarySqlTableModel : public QSqlTableModel
{
    Q_OBJECT

public:
    LibrarySqlTableModel(QWidget *parent = 0, QSqlDatabase database = QSqlDatabase());
    ~LibrarySqlTableModel();

    Qt::ItemFlags flags(const QModelIndex &index) const ;
    void setSelectStatement(QString queryString);
protected:
    QString selectStatementString;
    QString selectStatement() const ;
private:
    QItemSelectionModel *_selectionModel;
    QMimeData* mimeData(const QModelIndexList &indexes) const ;
    Qt::DropActions supportedDragActions() const ;
};

#endif // MODELS_H
