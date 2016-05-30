#include <iostream>
#include <librarywidgets.h>

#include <QApplication>
#include <QDrag>
#include <QHeaderView>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMenu>
#include <QAction>

LibraryTable::LibraryTable(QSqlDatabase database, QWidget *parent) : QTableView(parent)
{
    libraryDb = database;
    query = QSqlQuery(libraryDb);
    libraryModel = new LibrarySqlTableModel(this, libraryDb);
    libraryModel->setTable("Songs");
    setModel(libraryModel);
    while(libraryModel->canFetchMore())
    {
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

    setStyleSheet("QTableView::item::selected::active{\
                                                 color: black;\
                                                 background: qlineargradient(x1:1, y1:1, x2:1, y2:0, stop:0 #E0F0FF, stop:0.85 #E0F0FF, stop:1 white);\
                                                 border-top: 1px solid #A3D1FF;\
                                                 border-bottom: 1px solid #A3D1FF;\
                                                 }\
                  ");
}
LibraryTable::~LibraryTable()
{

}
void LibraryTable::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
    {
        qDebug() << "delete key";
    }
    else
    {
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
    if (event->buttons() & Qt::LeftButton)
    {
        int distance = (event->pos() - startDragPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
        {
            QDrag *drag = new QDrag(this);
            QModelIndexList selectedRows = this->selectionModel()->selectedRows();
            QMimeData *mimeData = this->model()->mimeData(selectedRows);
            drag->setMimeData(mimeData);
            drag->exec(Qt::CopyAction);
        }
    }
}
void LibraryTable::changeView(QString newView)
{
    query.prepare("SELECT Query, HeaderState FROM Views WHERE Name=(?)");
    query.addBindValue(newView);
    query.exec();
    query.next();
    qDebug() << query.record();
    libraryModel->setSelectStatement(query.value("Query").toString());
    reselectUpdate();
    horizontalHeader()->restoreState(query.value("HeaderState").toByteArray());  //Does not seem to break if ByteArray empty
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
    while(libraryModel->canFetchMore())
    {
        libraryModel->fetchMore();
    }
    viewport()->update();
}

LibraryTree::LibraryTree(QSqlDatabase &database, QWidget *parent) : QTreeView(parent)
{
    libraryDb = database;
    QSqlQuery query = QSqlQuery(libraryDb);
    currentView = "Music";
    QStandardItemModel *model = new QStandardItemModel();
    QStandardItem *rootItem = model->invisibleRootItem();
    setModel(model);
    header()->hide();

    query.exec("SELECT * from Views");
    while(query.next())
    {
        QStandardItem *item = new QStandardItem(query.value(1).toString());  //value(1) = name
        rootItem->appendRow(item);
    }
}
LibraryTree::~LibraryTree(){}
void LibraryTree::mouseReleaseEvent(QMouseEvent *event)
{
    if(this->indexAt(event->pos()).isValid())
    {
        emit newViewClicked(this->indexAt(event->pos()).data().toString());
    }
    currentView = this->indexAt(event->pos()).data().toString();
    QTreeView::mouseReleaseEvent(event);
}
void LibraryTree::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);
    QAction *modifyViewsAction = new QAction("Modify Views", menu);
    connect(modifyViewsAction, &QAction::triggered, this, &LibraryTree::emitRequestOptions);
    menu->addAction(modifyViewsAction);
    menu->exec(event->globalPos());
}
void LibraryTree::emitRequestOptions()
{
    emit requestOptions("Views");
    qDebug() << "emit";
}

LibrarySqlTableModel::LibrarySqlTableModel(QWidget *parent, QSqlDatabase libraryDb) : QSqlTableModel(parent, libraryDb)
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
    foreach (QModelIndex index, indexes)  //These indexes only include the first column. See LibraryWidget::mouseMoveEvent
    {
        rows.append(index.row());
    }
    qSort(rows.begin(), rows.end());
    for (int i = 0; i < rows.length(); i++)
    {
        IDSongList << this->record(rows[i]).value("IDSong").toInt();
    }
    stream << IDSongList;

    mimeData->setData("application/test", encodedData);

    return mimeData;
}
QString LibrarySqlTableModel::selectStatement() const
{
    return selectStatementString;
}
void LibrarySqlTableModel::setSelectStatement(QString queryString)
{
    selectStatementString = queryString;
}

Qt::DropActions LibrarySqlTableModel::supportedDragActions() const
{
    return Qt::CopyAction;
}
