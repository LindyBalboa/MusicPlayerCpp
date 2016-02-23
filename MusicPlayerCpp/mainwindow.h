#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtMultimedia/QMediaPlayer>
#include <QMainWindow>
#include <vlc/vlc.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QMediaPlayer *player;
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    libvlc_instance_t *instance;
    libvlc_media_player_t *mplayer;
    libvlc_media_t *media;
    int a;
};


#endif // MAINWINDOW_H
