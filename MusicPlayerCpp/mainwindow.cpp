#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtSql>
#include <iostream>
#include <QtMultimedia/QMediaPlayer>
#include "librarywidget.h"
#include <QActionGroup>
#include <vlc.h>
#include <iterator>
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setProperty("windowTitle","test");
    VLC* vlc = new VLC();
    qDebug("AUDIO OUTPUTS");
    QMenu* menuRight_Player_Devices;
    QMenu* menuLeft_Player_Devices;
    QAction* tempAction;
    QActionGroup* rightPlayerDeviceActionGroup = new QActionGroup(this);
    QActionGroup* leftPlayerDeviceActionGroup = new QActionGroup(this);
    QSignalMapper *deviceMenuSignalMapper = new QSignalMapper(this);

    menuRight_Player_Devices = menuBar()->findChild<QMenu *>("menuRight_Player_Devices");
    menuLeft_Player_Devices = menuBar()->findChild<QMenu *>("menuLeft_Player_Devices");

    devices = vlc->getDevices();
    QMap<QString,QString>::iterator i;
    for (i = devices.begin(); i != devices.end(); i++)
    {
        qDebug() << i.key() << " " << i.value() << endl;

        tempAction = menuRight_Player_Devices->addAction(i.key());
        rightPlayerDeviceActionGroup->addAction(tempAction);
        connect(tempAction,SIGNAL(triggered()),deviceMenuSignalMapper, SLOT(map()));
        deviceMenuSignalMapper->setMapping(tempAction,tempAction->text().insert(0,"R"));

        tempAction = menuLeft_Player_Devices->addAction(i.key());
        leftPlayerDeviceActionGroup->addAction(tempAction);
        connect(tempAction,SIGNAL(triggered()),deviceMenuSignalMapper, SLOT(map()));
        deviceMenuSignalMapper->setMapping(tempAction,tempAction->text().insert(0,"L"));
    }
    connect(deviceMenuSignalMapper, static_cast<void (QSignalMapper::*)(const QString &)>(&QSignalMapper::mapped),vlc, &VLC::handleDeviceChange);

    //
    //Set up library database and table
    //NOTE: Should try and move to librarywidget.cpp

}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
}

void MainWindow::on_pushButton_5_clicked()
{
}


