#include "mainwindow.h"
#include "filesystemscanner.h"
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
#include <QHeaderView>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    libraryDb = QSqlDatabase::addDatabase("QSQLITE");  //This connection gets passed along to any function or class that needs it via reference argument.
    libraryDb.setDatabaseName("database.db");
    if (libraryDb.open()){
    }else{
    }
    query = QSqlQuery(libraryDb);
    buildDatabase();

    QWidget *centralWidget = new QWidget;
    centralWidget->setContentsMargins(-10,-10,-10,-10);
    setCentralWidget(centralWidget);
    setGeometry(50,50,600,600);
    setWindowTitle("Title");

//    this->setStyleSheet("QSplitter::handle#libraryTreeTableSplitter {border: 1px solid rgb(240,240,250);\
//                                                                              width:  0px;\
//                                                                              }\
//                                 QTableView::item::selected::active {color: black;\
//                                                                     background: qlineargradient(x1:1, y1:1, x2:1, y2:0, stop:0 #E0F0FF, stop:0.85 #E0F0FF, stop:1 white);\
//                                                                     border-top: 1px solid #A3D1FF;\
//                                                                     border-bottom: 1px solid #A3D1FF;\
//                                                                     }\
//                                 QTableView::item {color: black;}\
//                  ");


    optionsWidget = new OptionsWidget(libraryDb, this);
    optionsWidget->show();

    searchDialog = new SearchDialog(this);
    searchDialog->show();

    QMenu *fileMenu = menuBar()->addMenu("&File");
        QAction *databaseScanAction = fileMenu->addAction("Add files to Library");
        connect(databaseScanAction, &QAction::triggered, this, &this->scanDatabase);
    QMenu *editMenu = menuBar()->addMenu("&Edit");
        QAction *optionsAction = editMenu->addAction("Options");
        connect(optionsAction, &QAction::triggered, [this](){this->optionsWidget->show();});
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
    libraryTree = new LibraryTree(libraryDb, this);
        libraryTreeTableSplitter->addWidget(libraryTree);
        libraryTree->setContentsMargins(0,0,-1,0);
        libraryTree->setMinimumWidth(20);
        connect(libraryTree, &LibraryTree::requestOptions, [this](QString menu){this->optionsWidget->show();});
    libraryTable = new LibraryTable(libraryDb, this);
        libraryTreeTableSplitter->addWidget(libraryTable);
        libraryTable->setContentsMargins(-1,0,0,0);
        connect(libraryTree,&LibraryTree::newViewClicked, libraryTable, &LibraryTable::changeView);
        connect(libraryTable->horizontalHeader(), &QHeaderView::sectionMoved,[this](int a,int b,int c){this->libraryTable->saveColumnOrder(this->libraryTree->currentView);});
    libraryTreeTableSplitter->setStretchFactor(0,0);
    libraryTreeTableSplitter->setStretchFactor(1,1);
    QList<int> size; size << 100 << 1;
    libraryTreeTableSplitter->setSizes(size);
    QSplitter *playerSplitter = new QSplitter();
        playerSplitter->setContentsMargins(-8,-9,-8,-8);
    centralSplitter->addWidget(playerSplitter);
    leftPlayer = new PlayerWidget("left", libraryDb, this);
        playerSplitter->addWidget(leftPlayer);
        leftPlayer->setContentsMargins(0,0,-9,0);
    rightPlayer = new PlayerWidget("Right", libraryDb, this);
        playerSplitter->addWidget(rightPlayer);
        rightPlayer->setContentsMargins(-9,0,0,0);


    this->statusBar()->setFixedHeight(20);
    progressBar->setMinimum(0);
    this->statusBar()->addWidget(progressBar);
    progressBar->hide();

    //qDebug("Finding AUDIO OUTPUTS and adding to drop down menu dynamically");
    QAction* tempAction;
    QActionGroup* rightPlayerDeviceActionGroup = new QActionGroup(this);
    QActionGroup* leftPlayerDeviceActionGroup = new QActionGroup(this);
    QSignalMapper *deviceMenuSignalMapperLeft = new QSignalMapper(this);
    QSignalMapper *deviceMenuSignalMapperRight = new QSignalMapper(this);
    devices = leftPlayer->vlc->getDevices();
    QMap<QString,QString>::iterator i;  //Build the device menus dynamically
    for (i = devices.begin(); i != devices.end(); i++){
        tempAction = rightPlayerDeviceMenu->addAction(i.key());
        rightPlayerDeviceActionGroup->addAction(tempAction);
        connect(tempAction,&QAction::triggered, deviceMenuSignalMapperRight, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        deviceMenuSignalMapperRight->setMapping(tempAction,tempAction->text());
        connect(deviceMenuSignalMapperRight, static_cast<void (QSignalMapper::*)(const QString &)>(&QSignalMapper::mapped),rightPlayer->vlc, &VLC::handleDeviceChange); //Maps all device actions to one handler

        tempAction = leftPlayerDeviceMenu->addAction(i.key());
        leftPlayerDeviceActionGroup->addAction(tempAction);
        connect(tempAction, &QAction::triggered, deviceMenuSignalMapperLeft, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        deviceMenuSignalMapperLeft->setMapping(tempAction,tempAction->text());
        connect(deviceMenuSignalMapperLeft, static_cast<void (QSignalMapper::*)(const QString &)>(&QSignalMapper::mapped),leftPlayer->vlc, &VLC::handleDeviceChange); //Maps all device actions to one handler
    }
    this->show();
    libraryTable->changeView("Music");

    connect(optionsWidget, &OptionsWidget::finished, this, optionsUpdated);
}

MainWindow::~MainWindow()
{
}

MainWindow::optionsUpdated()
{
    libraryTree->populateTree();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
}

void MainWindow::scanDatabase()
{
    scanner = new FileSystemScanner(libraryDb);
    connect(scanner, &FileSystemScanner::finished, this, &MainWindow::scanDatabaseFinished);
    connect(scanner, &FileSystemScanner::totalFileCountSignal, this, &MainWindow::updateScannerTotalFileCount);
    connect(scanner, &FileSystemScanner::currentFileCountSignal, this, &MainWindow::updateScannerCurrentFileCount);
    this->progressBar->show();
}

void MainWindow::buildDatabase()
{
    libraryDb.transaction();
    query.exec("CREATE TABLE IF NOT EXISTS Songs("
                                                  "IDSong	INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                  "Album_Artist	TEXT,"
                                                  "Album	TEXT,"
                                                  "Artist	TEXT,"
                                                  "Bitrate TEXT,"
                                                  "BPM	INTEGER,"
                                                  "Channels TEXT,"
                                                  "Comment	TEXT,"
                                                  "Competition	INTEGER,"
                                                  "Composer	TEXT,"
                                                  "Custom_1	TEXT,"
                                                  "Custom_2	TEXT,"
                                                  "Custom_3	TEXT,"
                                                  "Custom_4	TEXT,"
                                                  "Custom_5	TEXT,"
                                                  "Date	TEXT,"
                                                  "Disc_Number	TEXT,"
                                                  "Filename	TEXT,"
                                                  "Genre	TEXT,"
                                                  "Grouping	TEXT,"
                                                  "Length	REAL,"
                                                  "Mood	TEXT,"
                                                  "Occasion	TEXT,"
                                                  "Original_Date	TEXT,"
                                                  "Path	TEXT,"
                                                  "Play_Count	INTEGER,"
                                                  "Quality	TEXT,"
                                                  "Rating	INTEGER,"
                                                  "Sample_Rate TEXT,"
                                                  "Tempo	TEXT,"
                                                  "Title	TEXT,"
                                                  "Track_Number	TEXT,"
                                                  "Track_Total	TEXT"
                                                  ")");
    query.exec("CREATE VIRTUAL TABLE IF NOT EXISTS Search USING fts4("
                                                  "IDSong	INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                  "Album_Artist	TEXT,"
                                                  "Album	TEXT,"
                                                  "Artist	TEXT,"
                                                  "Bitrate TEXT,"
                                                  "BPM	INTEGER,"
                                                  "Channels TEXT,"
                                                  "Comment	TEXT,"
                                                  "Competition	INTEGER,"
                                                  "Composer	TEXT,"
                                                  "Custom_1	TEXT,"
                                                  "Custom_2	TEXT,"
                                                  "Custom_3	TEXT,"
                                                  "Custom_4	TEXT,"
                                                  "Custom_5	TEXT,"
                                                  "Date	TEXT,"
                                                  "Disc_Number	TEXT,"
                                                  "Filename	TEXT,"
                                                  "Genre	TEXT,"
                                                  "Grouping	TEXT,"
                                                  "Length	REAL,"
                                                  "Mood	TEXT,"
                                                  "Occasion	TEXT,"
                                                  "Original_Date	TEXT,"
                                                  "Path	TEXT,"
                                                  "Play_Count	INTEGER,"
                                                  "Quality	TEXT,"
                                                  "Rating	INTEGER,"
                                                  "Sample_Rate TEXT,"
                                                  "Tempo	TEXT,"
                                                  "Title	TEXT,"
                                                  "Track_Number	TEXT,"
                                                  "Track_Total	TEXT"
                                                  ")");
    query.exec("CREATE TABLE IF NOT EXISTS Views(IDView INTEGER PRIMARY KEY AUTOINCREMENT,\
                                                 Name	TEXT UNIQUE,\
                                                 Query TEXT,\
                                                 HeaderState BLOB,\
                                                 Visible INTEGER DEFAULT 1 CHECK (Visible=NULL OR Visible=0 OR Visible=1),\
                                                 [Order] INTEGER UNIQUE)");
    query.exec("INSERT OR IGNORE into Views (Name, Query, [Order], Visible) VALUES ('Music','SELECT * FROM Songs',1,1),\
                                                                 ('Left', NULL, NULL, NULL),\
                                                                 ('Right', NULL, NULL, NULL)");
               qDebug() << query.lastError();
    query.exec("CREATE TABLE IF NOT EXISTS Left(IDNowPlaying INTEGER PRIMARY KEY UNIQUE,"
                                                "IDSong	INTEGER)");
    query.exec("CREATE TABLE IF NOT EXISTS Right(IDNowPlaying INTEGER PRIMARY KEY UNIQUE,"
                                                "IDSong	INTEGER)");
    libraryDb.commit();
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

void MainWindow::scanDatabaseFinished()
{
    this->progressBar->hide();
    this->progressBar->reset();
    this->libraryTable->reselectUpdate();
}
