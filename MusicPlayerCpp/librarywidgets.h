#ifndef MODELS_H
#define MODELS_H

#include <QItemSelectionModel>
#include <QPoint>
#include <QTableView>
#include <Qt>
#include <QtSql>
#include <QTreeView>
#include <QStandardItem>

class LibrarySqlTableModel;
class LibraryTable : public QTableView
{
    Q_OBJECT

    public:
        LibraryTable(QWidget *parent =0);
        ~LibraryTable();
        void changeView(QString queryString, QByteArray headerState);
        void reselectUpdate();

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

/*
 * Library Tree
 */
class LibraryTree : public QTreeView
{
    Q_OBJECT
public:
    LibraryTree(QWidget *parent=0);
    ~LibraryTree();
    QString currentView;
    void populateTree();
    void newSearchQuery(QString query);
    void newSmartPlaylistQuery(QString query);
public slots:
signals:
    void newViewClicked(QString queryString, QByteArray headerState);
    void requestOptions(QString option);
private:
    QStringList searchQueries;
    enum DataRoles {QueryRole = Qt::UserRole+1,
                    HeaderRole
                   };
protected:
    QSqlQuery query;
    QStandardItem *rootItem;
    void mouseReleaseEvent(QMouseEvent *event) ;
    void contextMenuEvent(QContextMenuEvent *);
    void emitRequestOptions();
};

class LibrarySqlTableModel : public QSqlTableModel
{
    Q_OBJECT

public:
    LibrarySqlTableModel(QSqlDatabase database, QWidget *parent = 0);
    ~LibrarySqlTableModel();

    Qt::ItemFlags flags(const QModelIndex &index) const ;
    void setSelectStatement(QString queryString);
protected:
    QString selectStatementString;
    QString selectStatement() const ;
private:
    int sortColumn;
    Qt::SortOrder sortOrder;
    QItemSelectionModel *_selectionModel;
    QMimeData* mimeData(const QModelIndexList &indexes) const ;
    Qt::DropActions supportedDragActions() const ;
    void setSort(int column, Qt::SortOrder order);
    QString orderByClause() const ;
};

#endif // MODELS_H
