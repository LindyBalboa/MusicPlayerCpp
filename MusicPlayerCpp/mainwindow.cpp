#include "mainwindow.h"
#include "librarywidget.h"
#include <iostream>
#include <vlc.h>

#include <iterator>
#include <QActionGroup>
#include <QHBoxLayout>
#include <QDebug>
#include <QMenuBar>
#include <QPushButton>
#include <QSplitter>
#include <QtSql>
#include <QtMultimedia/QMediaPlayer>
#include <QTableView>
#include <QTreeView>
#include <QVBoxLayout>
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QSqlDatabase libraryDb = QSqlDatabase::addDatabase("QSQLITE");
    libraryDb.setDatabaseName("database.db");
    if (libraryDb.open()){
        //qDebug("DB is connected.");
    }else{
        //qDebug("DB is not connected.");
    }
    QSqlQuery q(libraryDb);
    //qDebug() << "main"<< q.exec("DELETE FROM left");
   // qDebug() << q.lastError();
    //libraryDb.commit();
    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    setGeometry(50,50,600,600);
    setWindowTitle("Title");

    QMenu *fileMenu = menuBar()->addMenu("&File");
    QMenu *editMenu = menuBar()->addMenu("&Edit");
    QMenu *viewMenu = menuBar()->addMenu("&View");
    QMenu *audioMenu = menuBar()->addMenu("&Audio");
        QMenu *rightPlayerDeviceMenu = audioMenu->addMenu("Right Player Devices");
        QMenu *leftPlayerDeviceMenu = menuBar()->addMenu("Left Player Devices");
    QMenu *toolsMenu = menuBar()->addMenu("&Tools");
    QMenu *aboutMenu = menuBar()->addMenu("&About");

    QHBoxLayout *centralLayout = new QHBoxLayout(this);
    centralWidget->setLayout(centralLayout);

    QSplitter *centralSplitter = new QSplitter();
    centralSplitter->setOrientation(Qt::Vertical);
    centralLayout->addWidget(centralSplitter);

    QSplitter *libraryTreeTableSplitter = new QSplitter(centralSplitter);
        centralSplitter->addWidget(libraryTreeTableSplitter);
    QTreeView *libraryTree = new QTreeView(this);
        libraryTreeTableSplitter->addWidget(libraryTree);
    LibraryWidget *libraryWidget = new LibraryWidget(libraryDb, this);
        libraryTreeTableSplitter->addWidget(libraryWidget);
    libraryTreeTableSplitter->setStretchFactor(0,1);
    libraryTreeTableSplitter->setStretchFactor(1,2);

    QSplitter *playerSplitter = new QSplitter();
    centralSplitter->addWidget(playerSplitter);
    leftPlayer = new PlayerWidget("left", libraryDb);
    playerSplitter->addWidget(leftPlayer);
    rightPlayer = new PlayerWidget("right", libraryDb);
    playerSplitter->addWidget(rightPlayer);

    //qDebug("AUDIO OUTPUTS");
    QMenu* menuRight_Player_Devices;   //Prep to dynamically create device menus after device detection
    QMenu* menuLeft_Player_Devices;
    QAction* tempAction;
    QActionGroup* rightPlayerDeviceActionGroup = new QActionGroup(this);
    QActionGroup* leftPlayerDeviceActionGroup = new QActionGroup(this);
    QSignalMapper *deviceMenuSignalMapperLeft = new QSignalMapper(this);
    QSignalMapper *deviceMenuSignalMapperRight = new QSignalMapper(this);

    devices = leftPlayer->vlc->getDevices();
    QMap<QString,QString>::iterator i;  //Build the device menus dynamically
    for (i = devices.begin(); i != devices.end(); i++)
    {
        //qDebug() << i.key() << " " << i.value() << endl;

        tempAction = rightPlayerDeviceActionGroup->addAction(i.key());
        rightPlayerDeviceActionGroup->addAction(tempAction);
        connect(tempAction,SIGNAL(triggered()),deviceMenuSignalMapperRight, SLOT(map()));
        deviceMenuSignalMapperRight->setMapping(tempAction,tempAction->text());
        connect(deviceMenuSignalMapperRight, static_cast<void (QSignalMapper::*)(const QString &)>(&QSignalMapper::mapped),rightPlayer->vlc, &VLC::handleDeviceChange); //Maps all device actions to one handler

        tempAction = leftPlayerDeviceMenu->addAction(i.key());
        leftPlayerDeviceActionGroup->addAction(tempAction);
        connect(tempAction,SIGNAL(triggered()),deviceMenuSignalMapperLeft, SLOT(map()));
        deviceMenuSignalMapperLeft->setMapping(tempAction,tempAction->text());
        connect(deviceMenuSignalMapperLeft, static_cast<void (QSignalMapper::*)(const QString &)>(&QSignalMapper::mapped),leftPlayer->vlc, &VLC::handleDeviceChange); //Maps all device actions to one handler
    }


}

MainWindow::~MainWindow()
{
}

void MainWindow::on_pushButton_clicked()
{
}

void MainWindow::on_pushButton_5_clicked()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    leftPlayer->playlist->saveNowPlaying();
    rightPlayer->playlist->saveNowPlaying();
    QMainWindow::closeEvent(event);
}


