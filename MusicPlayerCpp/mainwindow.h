#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "playerwidget.h"
#include "filesystemscanner.h"
#include <string>

#include <QFileSystemModel>
#include <QMainWindow>
#include <QProgressBar>
#include <QSignalMapper>
#include <QSqlDatabase>
#include <QThread>
#include <QtMultimedia/QMediaPlayer>
#include "librarywidgets.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        PlayerWidget *leftPlayer;
        PlayerWidget *rightPlayer;
        LibraryTree *libraryTree;
        LibraryTable *libraryTable;

public slots:
protected:
        QProgressBar *progressBar = new QProgressBar();
        QSqlDatabase libraryDb;
        QSqlQuery query;
        FileSystemScanner *scanner;
        QThread thread;
        void scanDatabaseFinished();
        void updateScannerTotalFileCount(int Right);
        void updateScannerCurrentFileCount(int Right);
protected slots:
        void scanDatabase();
        void closeEvent(QCloseEvent *event);
    private:
        std::map <QString, QString> deviceMap;
        QMap<QString,QString> devices;
        void buildDatabase();
private slots:
        void on_pushButton_clicked();
        void on_pushButton_5_clicked();


};



#endif // MAINWINDOW_H
