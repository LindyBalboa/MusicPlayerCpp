#ifndef FILESYSTEMSCANNER_H
#define FILESYSTEMSCANNER_H

#include <QFileSystemModel>
#include <QModelIndexList>
#include <QThread>
#include <QWidget>

class CheckableFileSystemModel;

class FileSystemScanner : public QWidget
{
    Q_OBJECT
public:
    explicit FileSystemScanner(QWidget *parent = 0);

signals:
    void finished();
    int totalFileCountSignal(int totalFileCount);
    int currentFileCountSignal(int currentFileCount);
private:
    CheckableFileSystemModel *model;
public slots:
protected:
    void scanFolders();
    void closeEvent(QCloseEvent *event) ;
};


class CheckableFileSystemModel : public QFileSystemModel
{
    Q_OBJECT

public:
    explicit CheckableFileSystemModel(QObject *parent = 0);
    ~CheckableFileSystemModel();
    QModelIndexList checkedList;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
protected:
    QModelIndexList semiList;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
};

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QStringList _dirs);
    ~Worker();
    void doWork();
private:
    QStringList dirs;
signals:
    int totalFileCountSignal(int totalFileCount);
    int currentFileCountSignal(int currentFileCount);
    void finished();
};

#endif // FILESYSTEMSCANNER_H
