#include <librarywidget.h>
#include <QHeaderView>
#include <iostream>


LibraryWidget::LibraryWidget(QWidget *parent) : QTableView(parent)
{
    qDebug() << "library getting made";
    QSqlDatabase libraryDb = QSqlDatabase::addDatabase("QSQLITE");
    libraryDb.setDatabaseName("database.db");
    if (libraryDb.open()){
        qDebug("DB is connected.");
    }else{
        qDebug("DB is not connected.");
    }
    LibrarySqlTableModel *LibraryModel = new LibrarySqlTableModel(this,libraryDb);
    LibraryModel->setTable("table1");
    LibraryModel->select();
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

    setStyleSheet("QTableView::item::selected::active{\
                                                 color: black;\
                                                 background: qlineargradient(x1:1, y1:1, x2:1, y2:0, stop:0 #E0F0FF, stop:0.85 #E0F0FF, stop:1 white);\
                                                 border-top: 1px solid #A3D1FF;\
                                                 border-bottom: 1px solid #A3D1FF;\
                                                 }\
                  ");
    qDebug() << this->model();
}

LibraryWidget::~LibraryWidget()
{

}

LibrarySqlTableModel::LibrarySqlTableModel(QWidget *parent, QSqlDatabase database) : QSqlTableModel(parent, database)
{
    setEditStrategy(QSqlTableModel::OnFieldChange);
    setSort(0,Qt::AscendingOrder);

}
LibrarySqlTableModel::~LibrarySqlTableModel()
{
}
Qt::ItemFlags LibrarySqlTableModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
Qt::DropActions LibrarySqlTableModel::supportedDragActions() const
{
    return Qt::CopyAction;
}
QMimeData* LibrarySqlTableModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    QList<QString> records;
    QList<int> rows;
    foreach (QModelIndex index, indexes)
    {
        if (index.isValid())
        {
            if (rows.contains(index.row()) == false)
            {
                for (int i=0; i < this->record(index.row()).count(); i++)
                {
                    records << this->record(index.row()).fieldName(i) << this->record(index.row()).value(i).toString();
                }
                rows.append(index.row());
            }
        }
    }
    for (int i=0; i<records.size()-1; i+=2)
    {
        qDebug() << records[i] << records[i+1];
    }
    stream << records;
    mimeData->setData("application/test", encodedData);

    return mimeData;
}
