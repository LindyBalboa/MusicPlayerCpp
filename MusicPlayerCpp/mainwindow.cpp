#include "mainwindow.h"
#include <QDebug>
#include <QtSql>
#include <iostream>
#include <QtMultimedia/QMediaPlayer>
#include "librarywidget.h"
#include <QActionGroup>
#include <vlc.h>
#include <iterator>
#include <QMenuBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QTreeView>
#include <QTableView>
#include <QPushButton>
#include <playerwidget.h>
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
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
    LibraryWidget *libraryWidget = new LibraryWidget(this);
        libraryTreeTableSplitter->addWidget(libraryWidget);
    libraryTreeTableSplitter->setStretchFactor(0,1);
    libraryTreeTableSplitter->setStretchFactor(1,2);

    QSplitter *playerSplitter = new QSplitter();
    centralSplitter->addWidget(playerSplitter);
    PlayerWidget *left = new PlayerWidget("left");
    playerSplitter->addWidget(left);
    PlayerWidget *right = new PlayerWidget("right");
    playerSplitter->addWidget(right);

    VLC* vlc = new VLC();  //Initialize the VLC class which controls both LEFT and RIGHT players
    //qDebug("AUDIO OUTPUTS");
    QMenu* menuRight_Player_Devices;   //Prep to dynamically create device menus after device detection
    QMenu* menuLeft_Player_Devices;
    QAction* tempAction;
    QActionGroup* rightPlayerDeviceActionGroup = new QActionGroup(this);
    QActionGroup* leftPlayerDeviceActionGroup = new QActionGroup(this);
    QSignalMapper *deviceMenuSignalMapper = new QSignalMapper(this);

    devices = vlc->getDevices();
    QMap<QString,QString>::iterator i;  //Build the device menus dynamically
    for (i = devices.begin(); i != devices.end(); i++)
    {
        qDebug() << i.key() << " " << i.value() << endl;

        tempAction = rightPlayerDeviceActionGroup->addAction(i.key());
        rightPlayerDeviceActionGroup->addAction(tempAction);
        connect(tempAction,SIGNAL(triggered()),deviceMenuSignalMapper, SLOT(map()));
        deviceMenuSignalMapper->setMapping(tempAction,tempAction->text().insert(0,"R"));   //Insert R or L at front of device name to carry information on which player

        tempAction = leftPlayerDeviceMenu->addAction(i.key());
        leftPlayerDeviceActionGroup->addAction(tempAction);
        connect(tempAction,SIGNAL(triggered()),deviceMenuSignalMapper, SLOT(map()));
        deviceMenuSignalMapper->setMapping(tempAction,tempAction->text().insert(0,"L"));
    }
    connect(deviceMenuSignalMapper, static_cast<void (QSignalMapper::*)(const QString &)>(&QSignalMapper::mapped),vlc, &VLC::handleDeviceChange); //Maps all device actions to one handler


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


