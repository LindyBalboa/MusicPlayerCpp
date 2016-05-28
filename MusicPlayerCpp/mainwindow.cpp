#include "mainwindow.h"
#include "filesystemscanner.h"
#include "librarywidgets.h"
#include <iostream>
#include <iterator>
#include <vlc.h>

#include <QActionGroup>
#include <QDebug>
#include <QDialog>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QPushButton>
#include <QSplitter>
#include <QStandardPaths>
#include <QStatusBar>
#include <QTableView>
#include <QThread>
#include <QTreeView>
#include <QVBoxLayout>
#include <QtMultimedia/QMediaPlayer>
#include <QtSql>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    libraryDb = QSqlDatabase::addDatabase("QSQLITE");  //This connection gets passed along to any function or class that needs it via reference argument.
    libraryDb.setDatabaseName("database.db");
    if (libraryDb.open()){
    }else{
    }
    QSqlQuery query(libraryDb);
    query.exec("CREATE TABLE IF NOT EXISTS left(IDNowPlaying INTEGER PRIMARY KEY UNIQUE,"
                                                "IDSong	INTEGER)");
    query.exec("CREATE TABLE IF NOT EXISTS right(IDNowPlaying INTEGER PRIMARY KEY UNIQUE,"
                                                "IDSong	INTEGER)");

    QWidget *centralWidget = new QWidget;
    centralWidget->setContentsMargins(-10,-10,-10,-10);
    setCentralWidget(centralWidget);
    setGeometry(50,50,600,600);
    setWindowTitle("Title");

    centralWidget->setStyleSheet("QSplitter::handle#libraryTreeTableSplitter {border: 1px solid rgb(240,240,250);"
                                 "				                              width:  0px;"
                                 "					                          }"
                                 "");


    QMenu *fileMenu = menuBar()->addMenu("&File");
        QAction *databaseScanAction = fileMenu->addAction("Add files to Library");
        connect(databaseScanAction, &QAction::triggered, this, &this->databaseScan);
    QMenu *editMenu = menuBar()->addMenu("&Edit");
    QMenu *viewMenu = menuBar()->addMenu("&View");
    QMenu *audioMenu = menuBar()->addMenu("&Audio");
        QMenu *rightPlayerDeviceMenu = audioMenu->addMenu("Right Player Devices");
        QMenu *leftPlayerDeviceMenu = audioMenu->addMenu("Left Player Devices");
    QMenu *toolsMenu = menuBar()->addMenu("&Tools");
    QMenu *aboutMenu = menuBar()->addMenu("&About");

    QHBoxLayout *centralLayout = new QHBoxLayout(this);
        centralWidget->setLayout(centralLayout);
    QSplitter *centralSplitter = new QSplitter();
        centralSplitter->setOrientation(Qt::Vertical);
        centralLayout->addWidget(centralSplitter);
    QSplitter *libraryTreeTableSplitter = new QSplitter(centralSplitter);
        libraryTreeTableSplitter->setObjectName("libraryTreeTableSplitter");
        centralSplitter->addWidget(libraryTreeTableSplitter);
    LibraryTree *libraryTree = new LibraryTree(libraryDb, this);
        libraryTreeTableSplitter->addWidget(libraryTree);
        libraryTree->setContentsMargins(0,0,-1,0);
        libraryTree->setMinimumWidth(20);
    LibraryTable *libraryTable = new LibraryTable(libraryDb, this);
        libraryTreeTableSplitter->addWidget(libraryTable);
        libraryTable->setContentsMargins(-1,0,0,0);
    connect(libraryTree,&LibraryTree::newViewClicked, libraryTable, &LibraryTable::changeView);
    libraryTreeTableSplitter->setStretchFactor(0,0);
    libraryTreeTableSplitter->setStretchFactor(1,1);
    QList<int> size; size << 100 << 1;
    libraryTreeTableSplitter->setSizes(size);
    QSplitter *playerSplitter = new QSplitter();
        playerSplitter->setContentsMargins(-8,-9,-8,-8);
    centralSplitter->addWidget(playerSplitter);
    leftPlayer = new PlayerWidget("left", libraryDb);
        playerSplitter->addWidget(leftPlayer);
        leftPlayer->setContentsMargins(0,0,-9,0);
    rightPlayer = new PlayerWidget("right", libraryDb);
        playerSplitter->addWidget(rightPlayer);
        rightPlayer->setContentsMargins(-9,0,0,0);


    this->statusBar()->setFixedHeight(20);
    progressBar->setMinimum(0);
    this->statusBar()->addWidget(progressBar);
    progressBar->hide();

    //qDebug("Finding AUDIO OUTPUTS and adding to drop down menu dynamically");
    QMenu* menuRight_Player_Devices;  //Prep to dynamically create device menus after device detection
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
        tempAction = rightPlayerDeviceMenu->addAction(i.key());
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
    QMainWindow::closeEvent(event);
}

void MainWindow::databaseScan()
{
    scanner = new FileSystemScanner(libraryDb);
    connect(scanner, &FileSystemScanner::finished, this, &MainWindow::databaseScanFinished);
    connect(scanner, &FileSystemScanner::totalFileCountSignal, this, &MainWindow::updateScannerTotalFileCount);
    connect(scanner, &FileSystemScanner::currentFileCountSignal, this, &MainWindow::updateScannerCurrentFileCount);
    this->progressBar->show();
}

void MainWindow::updateScannerTotalFileCount(int count)
{
    this->progressBar->setMaximum(count);
}

void MainWindow::updateScannerCurrentFileCount(int count)
{
    this->progressBar->setValue(count);
    this->progressBar->update();
}

void MainWindow::databaseScanFinished()
{
    this->progressBar->hide();
    this->progressBar->reset();
}
