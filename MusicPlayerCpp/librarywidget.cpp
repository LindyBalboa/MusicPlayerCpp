#include <librarywidget.h>
#include <QtSql>
#include <Qt>

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
