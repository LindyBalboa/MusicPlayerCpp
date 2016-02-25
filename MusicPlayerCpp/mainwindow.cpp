#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtSql>
#include <iostream>
#include <QtMultimedia/QMediaPlayer>
#include "librarywidget.h"
#include "vlc/vlc.h"
#include <QActionGroup>

using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setProperty("windowTitle","test");


    const char * const vlc_args[] = {
             // "--verbose=2", //be much more verbose then normal for debugging purpose
               };
    instanceRight = libvlc_new(0,vlc_args);
    mediaRight = libvlc_media_new_location(instanceRight, "file:///C:/test.m4a");
    mPlayerRight = libvlc_media_player_new_from_media(mediaRight);
    libvlc_media_player_play(mPlayerRight);

    instanceLeft = libvlc_new(0,vlc_args);
    mediaLeft = libvlc_media_new_location(instanceLeft, "file:///C:/test.mp3");
    mPlayerLeft = libvlc_media_player_new_from_media(mediaLeft);
    libvlc_media_player_play(mPlayerLeft);


    qDebug("AUDIO OUTPUTS");
    libvlc_audio_output_device_t* devices;
    QString deviceName;
    QMenu* menuRight_Player_Devices;
    QMenu* menuLeft_Player_Devices;
    QAction* tempAction;
    QActionGroup* rightPlayerDeviceActionGroup = new QActionGroup(this);
    QActionGroup* leftPlayerDeviceActionGroup = new QActionGroup(this);
    QSignalMapper *deviceMenuSignalMapper = new QSignalMapper(this);

    menuRight_Player_Devices = menuBar()->findChild<QMenu *>("menuRight_Player_Devices");
    menuLeft_Player_Devices = menuBar()->findChild<QMenu *>("menuLeft_Player_Devices");
    devices = libvlc_audio_output_device_list_get(instanceLeft,"directx");
    while(devices!=NULL)
    {
       deviceName = devices->psz_description;
       deviceMap[deviceName] = devices->psz_device ;
       tempAction = menuRight_Player_Devices->addAction(deviceName);
       rightPlayerDeviceActionGroup->addAction(tempAction);
       connect(tempAction,SIGNAL(triggered()),deviceMenuSignalMapper, SLOT(map()));
       tempAction = menuLeft_Player_Devices->addAction(deviceName);
       leftPlayerDeviceActionGroup->addAction(tempAction);
       connect(tempAction,SIGNAL(triggered()),deviceMenuSignalMapper, SLOT(map()));
       devices = devices->p_next;

    }

    int numberOfDevices = deviceMap.size();
    for (int i=0; i < numberOfDevices ; i++)
    {
        //connect(menuLeft_Player_Devices->actions()[i],QAction::triggered,handleDeviceChange);
        deviceMenuSignalMapper->setMapping(menuLeft_Player_Devices->actions()[i], menuLeft_Player_Devices->actions()[i]->text().insert(0,"L") );
        deviceMenuSignalMapper->setMapping(menuRight_Player_Devices->actions()[i], menuRight_Player_Devices->actions()[i]->text().insert(0,"R") );
    }
    connect(deviceMenuSignalMapper,SIGNAL(mapped(QString)),this, SLOT(handleDeviceChange(QString)));
    //libvlc_audio_output_t* outputs;
    //outputs = libvlc_audio_output_list_get(instance);
    //while (outputs != NULL)
    //{
    //qDebug(outputs->psz_description);
    //qDebug(outputs->psz_name);
    //outputs = outputs->p_next;
   // }

    //player = new QMediaPlayer(this);
    //player->setMedia(QUrl::fromLocalFile("C:/test.mp3"));
    //player->setVolume(50);
    //player->play();

    QSqlDatabase library_db = QSqlDatabase::addDatabase("QSQLITE");
    library_db.setDatabaseName("database.db");
    if (library_db.open()){
        qDebug("DB is connected.");
    }else{
        qDebug("DB is not connected.");
    }
    LibrarySqlTableModel *LibraryModel = new LibrarySqlTableModel(this,library_db);
    LibraryModel->setTable("table1");
    LibraryModel->select();

    ui->LibraryTable->setModel(LibraryModel);
    ui->LibraryTable->show();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleDeviceChange(QString device)
{
    QChar whichPlayer = device[0];
    qDebug() << whichPlayer;
    device = device.remove(0,1);
    device = MainWindow::deviceMap[device];
    if (whichPlayer == 'R')
    {
        float pos = libvlc_media_player_get_position(mPlayerRight);
        libvlc_media_player_stop(mPlayerRight);
        libvlc_audio_output_device_set(mPlayerRight, "directx", device.toStdString().c_str());
        libvlc_media_player_play(mPlayerRight);
        libvlc_media_player_set_position(mPlayerRight,pos);
    }else if (whichPlayer == 'L')
    {
        float pos = libvlc_media_player_get_position(mPlayerLeft);
        libvlc_media_player_stop(mPlayerLeft);
        libvlc_audio_output_device_set(mPlayerLeft, "directx", device.toStdString().c_str());
        libvlc_media_player_play(mPlayerLeft);
        libvlc_media_player_set_position(mPlayerLeft,pos);
    }
}
void MainWindow::on_pushButton_clicked()
{
}

void MainWindow::on_pushButton_5_clicked()
{
}


