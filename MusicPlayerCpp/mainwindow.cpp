#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtSql>
#include <iostream>
#include <QtMultimedia/QMediaPlayer>
#include "librarywidget.h"
#include "vlc/vlc.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setProperty("windowTitle","test");
    a = 1;

    const char * const vlc_args[] = {
              "--verbose=2", //be much more verbose then normal for debugging purpose
               };
    instance = libvlc_new(1,vlc_args);
    media = libvlc_media_new_location(instance, "file:///C:/test.m4a");
    mplayer = libvlc_media_player_new_from_media(media);
    libvlc_media_player_play(mplayer);

    libvlc_audio_output_t* outputs;
    outputs = libvlc_audio_output_list_get(instance);

    qDebug("AUDIO OUTPUTS");
    libvlc_audio_output_device_t* devices;
    devices = libvlc_audio_output_device_list_get(instance,"directx");
    while(devices!=NULL)
    {
        qDebug(devices->psz_description);
         qDebug(devices->psz_device);
        devices = devices->p_next;
    }
    while (outputs != NULL)
    {
    qDebug(outputs->psz_description);
    qDebug(outputs->psz_name);
    outputs = outputs->p_next;
    }

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

void MainWindow::on_pushButton_clicked()
{
    a=!a;
    libvlc_media_player_set_pause(mplayer,a);
  }

void MainWindow::on_pushButton_5_clicked()
{
    if (a==1)
    {
        libvlc_media_player_stop(mplayer);
        libvlc_audio_output_device_set(mplayer,"directx","{A7B4C48E-1BA6-4B2E-9427-EC8375E01FD5}");
        libvlc_media_player_play(mplayer);
        a=0;
    }else
    {
        libvlc_media_player_stop(mplayer);
        libvlc_audio_output_device_set(mplayer,"directx","{9F307A36-1A93-450C-88C9-4F443893B24D}");
        libvlc_media_player_play(mplayer);
        a=1;
    }
}


