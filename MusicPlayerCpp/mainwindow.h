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

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        PlayerWidget *leftPlayer;
        PlayerWidget *rightPlayer;

public slots:
protected:
        QProgressBar *progressBar = new QProgressBar();
        QSqlDatabase libraryDb;
        FileSystemScanner *scanner;
        QThread thread;
        void databaseScanFinished();
        void updateScannerTotalFileCount(int count);
        void updateScannerCurrentFileCount(int count);
protected slots:
        void databaseScan();
        void closeEvent(QCloseEvent *event); Q_DECL_OVERRIDE
    private:
        std::map <QString, QString> deviceMap;
        QMap<QString,QString> devices;
    private slots:
        void on_pushButton_clicked();
        void on_pushButton_5_clicked();


};



#endif // MAINWINDOW_H
