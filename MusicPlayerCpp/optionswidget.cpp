#include "optionswidget.h"
#include "globals.h"
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QHeaderView>
#include <QSizePolicy>
#include <QPushButton>
#include <QDebug>
#include <QCheckBox>
#include <QApplication>

OptionsWidget::OptionsWidget(QWidget *parent, Qt::WindowFlags flags) : QWidget(parent, flags)
{
    setWindowTitle("Options");
    this->setStyleSheet("QStackedWidget {border-left: 1px solid white;"
                        "				 border-top: 1px solid white;"
                        "				 border-right: 1px solid gray;"
                        "				 border-bottom: 1px solid gray;}");
    QVBoxLayout *mainVLayout = new QVBoxLayout(this);
        this->setLayout(mainVLayout);
    QHBoxLayout *treeStackLayout = new QHBoxLayout(this);
        mainVLayout->addLayout(treeStackLayout);
        QTreeView *treeView = new QTreeView(this);
        treeStackLayout->addWidget(treeView);
        treeView->header()->hide();
        treeView->setFixedWidth(100);
        QStandardItemModel *model = new QStandardItemModel(treeView);
        QStandardItem *rootItem = model->invisibleRootItem();
        treeView->setModel(model);
        QStandardItem *views = new QStandardItem("Views");
        rootItem->appendRow(views);

        QStackedWidget *stackedWidget = new QStackedWidget(this);
        treeStackLayout->addWidget(stackedWidget);
        ViewsPanel *viewsPanel = new ViewsPanel(this);
        stackedWidget->addWidget(viewsPanel);
        connect(viewsPanel, &ViewsPanel::requestViewDialog, this, [this](){emit this->requestViewDialog();});
    QHBoxLayout *buttonLayout = new QHBoxLayout(this);
        QPushButton *finishedButton = new QPushButton("Finished");
        finishedButton->setFixedSize(finishedButton->sizeHint());
        buttonLayout->addWidget(finishedButton, 0, Qt::AlignRight);
        connect(finishedButton, &QPushButton::clicked, this, &OptionsWidget::finishedClicked);
    mainVLayout->addLayout(buttonLayout);
}
OptionsWidget::~OptionsWidget(){}
void OptionsWidget::closeEvent(QCloseEvent *event)
{
    this->hide();
}
void OptionsWidget::finishedClicked()
{
    this->hide();
    emit finished();
}
ViewsPanel::ViewsPanel(QWidget *parent) : QWidget(parent)
{
    query = QSqlQuery(libraryDb);
    QVBoxLayout *centralLayout = new QVBoxLayout(this);
    setLayout(centralLayout);

    tableView = new QTableView(this);
        centralLayout->addWidget(tableView);
        tableModel = new SqlTableModel(this, libraryDb);
        tableModel->select();
        tableView->setModel(tableModel);

        tableView->setShowGrid(false);
        tableView->verticalHeader()->hide();
        tableView->verticalHeader()->setDefaultSectionSize(15);
        tableView->horizontalHeader()->setFixedHeight(20);
        tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        //tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
        //tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QHBoxLayout * buttonLayout = new QHBoxLayout(this);
        centralLayout->addLayout(buttonLayout);
        QPushButton *addButton = new QPushButton("Add", this);
        connect(addButton, &QPushButton::clicked, [this](){emit this->requestViewDialog();});
        buttonLayout->addWidget(addButton);
        addButton->setFocusPolicy(Qt::TabFocus);
        QPushButton *deleteButton = new QPushButton("Delete", this);
        buttonLayout->addWidget(deleteButton);
        connect(deleteButton, &QPushButton::clicked, this, &ViewsPanel::deleteView);
        deleteButton->setFocusPolicy(Qt::TabFocus);
        QPushButton *editButton = new QPushButton("Edit", this);
        buttonLayout->addWidget(editButton);
        connect(editButton, &QPushButton::clicked, this, &ViewsPanel::editView);
        editButton->setFocusPolicy(Qt::TabFocus);
        QPushButton *upButton = new QPushButton("Move up", this);
        buttonLayout->addWidget(upButton);
        connect(upButton, &QPushButton::clicked, this, &ViewsPanel::moveViewUp);
        upButton->setFocusPolicy(Qt::TabFocus);
        QPushButton *downButton = new QPushButton("Move down", this);
        buttonLayout->addWidget(downButton);
        connect(downButton, &QPushButton::clicked, this, &ViewsPanel::moveViewDown);
        downButton->setFocusPolicy(Qt::TabFocus);
}
ViewsPanel::~ViewsPanel(){};

void ViewsPanel::deleteView(){

}
void ViewsPanel::editView(){

}
void ViewsPanel::moveViewUp(){
    if(tableView->selectionModel()->hasSelection()){
        if (tableView->selectionModel()->selectedRows()[0].row()==0)
            return;
        int rowToMove = 1 + tableView->selectionModel()->selectedRows()[0].row();  //Sqlite is not zero indexed
        libraryDb.transaction();
        query.exec("UPDATE Views SET [Order]=(-[Order]-1) WHERE [Order]=" + QString::number(rowToMove-1) );
        query.exec("UPDATE Views SET [Order]=[Order]-1 WHERE [Order]=" + QString::number(rowToMove) );
        query.exec("UPDATE Views SET [Order]=-[Order] WHERE [Order]<0");
        query.clear();
        libraryDb.commit();
        tableModel->select();
        tableView->selectionModel()->select(tableModel->index(rowToMove-2,0), QItemSelectionModel::Select|QItemSelectionModel::Rows);
        tableView->viewport()->update();
    }
}
void ViewsPanel::moveViewDown(){
    if(tableView->selectionModel()->hasSelection()){
        if (tableView->selectionModel()->selectedRows()[0].row()==tableModel->rowCount()-1)
            return;
        int rowToMove = 1 + tableView->selectionModel()->selectedRows()[0].row();  //Sqlite is not zero indexed
        libraryDb.transaction();
        query.exec("UPDATE Views SET [Order]=(-[Order]+1) WHERE [Order]=" + QString::number(rowToMove+1) );
        query.exec("UPDATE Views SET [Order]=[Order]+1 WHERE [Order]=" + QString::number(rowToMove) );
        query.exec("UPDATE Views SET [Order]=-[Order] WHERE [Order]<0");
        query.clear();
        libraryDb.commit();
        tableModel->select();
        tableView->selectionModel()->select(tableModel->index(rowToMove,0), QItemSelectionModel::Select|QItemSelectionModel::Rows);
        //tableView->viewport()->update();
    }
}

SqlTableModel::SqlTableModel(QObject *parent, QSqlDatabase &database) : QSqlTableModel(parent, database)
{
}
SqlTableModel::~SqlTableModel(){};
QVariant SqlTableModel::data(const QModelIndex &index, int role) const
{
    this->query().seek(index.row());
    if(role==Qt::CheckStateRole & index.column()==0){
        return (this->query().value(0).toBool()?Qt::Checked:Qt::Unchecked);
    }
    if(role==Qt::DisplayRole & index.column()==0){
    }else{
        return QSqlTableModel::data(index, role);
    }
}
bool SqlTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role==Qt::CheckStateRole){
        libraryDb.transaction();
        QSqlQuery query(libraryDb);
        query.prepare("UPDATE Views SET Visible=(?) where Name=(?)");
        query.addBindValue(QVariant((value.toInt()==Qt::Checked)?1:0));
        query.addBindValue(this->index(index.row(),index.column()+1).data());
        query.exec();
        query.clear();
        emit dataChanged(index,index);
        libraryDb.commit();
        this->select();
        return true;
    }
    return false;
}
Qt::ItemFlags SqlTableModel::flags(const QModelIndex &index) const
{
    if(index.column()==0){
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable| Qt::ItemIsUserCheckable;
    }else{
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
}
QString SqlTableModel::selectStatement() const
{
    return "SELECT Visible, Name from Views WHERE Visible IS NOT NULL ORDER BY [Order]";
}
