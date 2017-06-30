#include <iostream>
#include <librarywidgets.h>
#include "globals.h"

#include <QApplication>
#include <QDrag>
#include <QHeaderView>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QMenu>
#include <QAction>

/*
 * Library Table class
 *
 * Needs: Should know if it is looking at a search/View or a playlist. Playlists need to be drag droppable
 */
LibraryTable::LibraryTable(QWidget *parent) : QTableView(parent)
{
    query = QSqlQuery(libraryDb);
    libraryModel = new LibrarySqlTableModel(libraryDb, this);
    libraryModel->setTable("Songs");
    setModel(libraryModel);
    while(libraryModel->canFetchMore()){
        libraryModel->fetchMore();
    }
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    verticalHeader()->hide();
    verticalHeader()->setDefaultSectionSize(15);
    horizontalHeader()->setSectionsMovable(true);
    horizontalHeader()->setFixedHeight(20);
    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    horizontalHeader()->setHighlightSections(false);
    setDragEnabled(true);
    setShowGrid(false);
    setSortingEnabled(true);
    sortByColumn(2, Qt::DescendingOrder);
    horizontalHeader()->hideSection(0);
    horizontalHeader()->moveSection(30,0);

//    setStyleSheet("QTableView::item::selected::active{\
//                                                 color: black;\
//                                                 background: qlineargradient(x1:1, y1:1, x2:1, y2:0, stop:0 #E0F0FF, stop:0.85 #E0F0FF, stop:1 white);\
//                                                 border-top: 1px solid #A3D1FF;\
//                                                 border-bottom: 1px solid #A3D1FF;\
//                                                 }\
//                  ");
}
LibraryTable::~LibraryTable()
{

}
void LibraryTable::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete){
        qDebug() << "delete key";
    }else{
        qDebug() << "another key";
        QWidget::keyPressEvent(event);
    }
}
void LibraryTable::mousePressEvent(QMouseEvent *event)
{
    startDragPos = event->pos();  //Needed for Manhatten length
    QTableView::mousePressEvent(event);
}
void LibraryTable::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton){
        int distance = (event->pos() - startDragPos).manhattanLength();
        if (distance >= QApplication::startDragDistance()){
            QDrag *drag = new QDrag(this);
            QModelIndexList selectedRows = this->selectionModel()->selectedRows();
            QMimeData *mimeData = this->model()->mimeData(selectedRows);
            drag->setMimeData(mimeData);
            drag->exec(Qt::CopyAction);
        }
    }
}
void LibraryTable::changeView(QString queryString, QByteArray headerState)
{
    libraryModel->setSelectStatement(queryString);
    reselectUpdate();
    //horizontalHeader()->restoreState(headerState);  //Does not seem to break if ByteArray empty
}

void LibraryTable::saveColumnOrder(QString viewName)
{
    query.prepare("UPDATE Views SET HeaderState=(?) WHERE Name=(?)");
    query.addBindValue(horizontalHeader()->saveState());
    query.addBindValue(viewName);
    query.exec();

}

void LibraryTable::reselectUpdate()
{
    libraryModel->select();
    while(libraryModel->canFetchMore()){
        libraryModel->fetchMore();
    }
    viewport()->update();
}

/*
 * Library Tree class
 */
LibraryTree::LibraryTree(QWidget *parent) : QTreeView(parent)
{
    query = QSqlQuery(libraryDb);
    QStandardItemModel *model = new QStandardItemModel();
    setModel(model);
    header()->hide();
    populateTree();
}
LibraryTree::~LibraryTree(){}
void LibraryTree::mouseReleaseEvent(QMouseEvent *event)
{
    QModelIndex index = this->indexAt(event->pos());
    if(index.isValid() & event->button()==Qt::LeftButton){
        emit newViewClicked(index.data(LibraryTree::QueryRole).toString(),
                            index.data(LibraryTree::HeaderRole).toByteArray());
    }
    QTreeView::mouseReleaseEvent(event);
}
void LibraryTree::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);
    QAction *modifyViewsAction = new QAction("Modify Views", menu);
    connect(modifyViewsAction, &QAction::triggered, this, [this](){emit requestOptions("Views");});
    menu->addAction(modifyViewsAction);
    menu->exec(event->globalPos());
}
void LibraryTree::populateTree()
{
    rootItem = static_cast<QStandardItemModel*>(model())->invisibleRootItem();
    rootItem->removeRows(0, rootItem->rowCount());
    query.exec("SELECT * from Views WHERE Visible==1 ORDER BY [Order]");
    while(query.next()){
            QStandardItem *item = new QStandardItem(query.value("Name").toString());  //value(1) = name
            item->setData(query.value("Query").toString(), LibraryTree::QueryRole);
            item->setData(query.value("HeaderState").toByteArray(), LibraryTree::HeaderRole);
            rootItem->appendRow(item);
    }
    /*
     * to do: add searches to DB
     */
    if (searchQueries.size() > 0){
        QStandardItem *searchItem = new QStandardItem("Searches");
        rootItem->appendRow(searchItem);
        foreach(QString queryString, searchQueries){
            QStandardItem *item = new QStandardItem(queryString);  //value(1) = name
            item->setData(queryString, LibraryTree::QueryRole);
            //item->setData(, LibraryTree::HeaderRole);
            searchItem->appendRow(item);
        }
    }
}
void LibraryTree::newSearchQuery(QString queryString)
{
    if (searchQueries.count()>6){
        searchQueries.removeFirst();
    }

    searchQueries << queryString;
    populateTree();
    QStandardItem *searchItem = rootItem->child(rootItem->rowCount()-1);
    QStandardItem *thisSearch = searchItem->child(searchItem->rowCount()-1);
    emit newViewClicked(thisSearch->data(LibraryTree::QueryRole).toString(),
                        searchItem->data(LibraryTree::HeaderRole).toByteArray());
    this->selectionModel()->select(thisSearch->index(), QItemSelectionModel::Select);
    this->expand(searchItem->index());
}
void LibraryTree::newSmartPlaylistQuery(QString query)
{
    qDebug() << "new playlist with query:" + query;
}
LibrarySqlTableModel::LibrarySqlTableModel(QSqlDatabase libraryDb, QWidget *parent) : QSqlTableModel(parent, libraryDb)
{
    selectStatementString = "SELECT * from Songs";
}
LibrarySqlTableModel::~LibrarySqlTableModel()
{
}

Qt::ItemFlags LibrarySqlTableModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
QMimeData* LibrarySqlTableModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    QList<int> rows;
    QVariantList IDSongList;
    foreach (QModelIndex index, indexes) { //These indexes only include the first column. See LibraryWidget::mouseMoveEvent
        rows.append(index.row());
    }
    qSort(rows.begin(), rows.end());
    for (int i = 0; i < rows.length(); i++){
        IDSongList << this->record(rows[i]).value("IDSong").toInt();
    }
    stream << IDSongList;

    mimeData->setData("application/test", encodedData);

    return mimeData;
}
void LibrarySqlTableModel::setSort(int column, Qt::SortOrder order)
{
    sortColumn = column;
    sortOrder = order;
}
QString LibrarySqlTableModel::orderByClause() const
{
    QSqlField field = record().field(sortColumn);
    if (!field.isValid())
        return QString();

    if (sortOrder == Qt::AscendingOrder){
        return "ORDER BY " + field.name() + " ASC";
    } else if (sortOrder == Qt::DescendingOrder){
        return "ORDER BY " + field.name() + " DESC";
    }
}
QString LibrarySqlTableModel::selectStatement() const
{
    return selectStatementString + " " + this->orderByClause();
}
void LibrarySqlTableModel::setSelectStatement(QString queryString)
{
    selectStatementString = queryString;
}

Qt::DropActions LibrarySqlTableModel::supportedDragActions() const
{
    return Qt::CopyAction;
}
