#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtMultimedia/QMediaPlayer>
#include <QMainWindow>
#include <QSignalMapper>
#include <string>
#include <QSqlDatabase>
#include "playerwidget.h"
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
private slots:
    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

private:
    std::map <QString, QString> deviceMap;
    QMap<QString,QString> devices;

};


#endif // MAINWINDOW_H
