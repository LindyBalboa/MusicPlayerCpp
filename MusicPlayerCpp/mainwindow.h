#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "playerwidget.h"
#include <string>

#include <QMainWindow>
#include <QSignalMapper>
#include <QSqlDatabase>
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
    protected slots:
        void closeEvent(QCloseEvent *event); Q_DECL_OVERRIDE
    private:
        std::map <QString, QString> deviceMap;
        QMap<QString,QString> devices;
    private slots:
        void on_pushButton_clicked();
        void on_pushButton_5_clicked();


};


#endif // MAINWINDOW_H
