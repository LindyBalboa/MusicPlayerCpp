#include <librarywidget.h>


LibraryWidget::LibraryWidget(QWidget *parent) : QTableView(parent)
{
    qDebug() << "library getting made";
    QSqlDatabase library_db = QSqlDatabase::addDatabase("QSQLITE");
    library_db.setDatabaseName("database.db");
    if (library_db.open()){
        qDebug("DB is connected.");
    }else{
        qDebug("DB is not connected.");
    }
    LibrarySqlTableModel *LibraryModel = new LibrarySqlTableModel(this,library_db);
    LibraryModel->setTable("table1");
    LibraryModel->select();
    setModel(LibraryModel);
}

LibraryWidget::~LibraryWidget()
{

}

LibrarySqlTableModel::LibrarySqlTableModel(QWidget *parent, QSqlDatabase database) : QSqlTableModel(parent, database)
{
    this->setEditStrategy(QSqlTableModel::OnFieldChange);
    this->setSort(0,Qt::AscendingOrder);

}
LibrarySqlTableModel::~LibrarySqlTableModel()
{
}
Qt::ItemFlags LibrarySqlTableModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
