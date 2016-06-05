#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <QWidget>
#include <QtSql>
#include <QStyledItemDelegate>
#include <QTableView>

class ViewsPanel;

class OptionsWidget : public QWidget
{
    Q_OBJECT
public:
    OptionsWidget(QSqlDatabase &database, QWidget *parent=0, Qt::WindowFlags flags=Qt::Window);
    QSqlDatabase libraryDb;
    QSqlQuery query;
    ViewsPanel *viewsPanel;
    ~OptionsWidget();
    void finishedClicked();
signals:
    void finished();
protected:
private:
    virtual void closeEvent(QCloseEvent *event);
};

class ViewsPanel : public QWidget
{
    Q_OBJECT
public:
    ViewsPanel(QSqlDatabase &database, QWidget *parent=0);
    ~ViewsPanel();

    QSqlTableModel *tableModel;
    save();
private:
    QSqlDatabase libraryDb;
    QSqlQuery query;
    QTableView *tableView;
    void addView();
    void deleteView();
    void editView();
    void moveViewUp();
    void moveViewDown();
};

class SqlTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    SqlTableModel(QObject *parent, QSqlDatabase &database);
    ~SqlTableModel();

    QSqlDatabase libraryDb;
private:
    QString selectStatement() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
};

#endif // OPTIONSWIDGET_H
