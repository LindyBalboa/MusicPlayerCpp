#include <iostream>
#include <librarywidget.h>

#include <QHeaderView>


LibraryWidget::LibraryWidget(QSqlDatabase database, QWidget *parent) : QTableView(parent)
{
    qDebug() << "library getting made";
    libraryDb = database;
    LibrarySqlTableModel *LibraryModel = new LibrarySqlTableModel(this, libraryDb);
    setModel(LibraryModel);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    verticalHeader()->hide();
    verticalHeader()->setDefaultSectionSize(15);
    horizontalHeader()->setSectionsMovable(true);
    horizontalHeader()->setFixedHeight(20);
    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setDragEnabled(true);
    setShowGrid(false);
    setSortingEnabled(true);
    sortByColumn(2, Qt::DescendingOrder);

    setStyleSheet("QTableView::item::selected::active{\
                                                 color: black;\
                                                 background: qlineargradient(x1:1, y1:1, x2:1, y2:0, stop:0 #E0F0FF, stop:0.85 #E0F0FF, stop:1 white);\
                                                 border-top: 1px solid #A3D1FF;\
                                                 border-bottom: 1px solid #A3D1FF;\
                                                 }\
                  ");
}
LibraryWidget::~LibraryWidget()
{

}

LibrarySqlTableModel::LibrarySqlTableModel(QWidget *parent, QSqlDatabase libraryDb) : QSqlQueryModel(parent)
{
    setQuery("SELECT * FROM table1", libraryDb);
    while(canFetchMore())
    {
        fetchMore();
    }
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
    foreach (QModelIndex index, indexes)
    {
        if (index.isValid())
        {
            if (rows.contains(index.row()) == false)
            {
                rows.append(index.row());
            }
        }
    }
    qSort(rows.begin(),rows.end());
    stream << rows.size();
    for (int i = rows.size()-1; i>=0; i--)
    {
        stream << this->record(rows[i]).value("IDSong").toInt();
    }

    mimeData->setData("application/test", encodedData);

    return mimeData;
}
Qt::DropActions LibrarySqlTableModel::supportedDragActions() const
{
    return Qt::CopyAction;
}
