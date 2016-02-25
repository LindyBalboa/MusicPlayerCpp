#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtMultimedia/QMediaPlayer>
#include <QMainWindow>
#include <vlc/vlc.h>
#include <QSignalMapper>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    libvlc_instance_t *instanceRight;
    libvlc_media_player_t *mPlayerRight;
    libvlc_media_t *mediaRight;
    libvlc_instance_t *instanceLeft;
    libvlc_media_player_t *mPlayerLeft;
    libvlc_media_t *mediaLeft;
    std::map <QString, QString> deviceMap;

public slots:
    void handleDeviceChange(QString device);
};


#endif // MAINWINDOW_H
