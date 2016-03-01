#ifndef VLC_H
#define VLC_H
#include <map>
#include <string>
#include <QString>
#include "vlc/vlc.h"
#include <QMap>
#include <QObject>

using namespace std;

class VLC : public QObject
{
    Q_OBJECT

public:
    VLC(QObject *parent = 0);
    QMap<QString, QString> getDevices();
    void play(libvlc_media_player_t* player);
    void pause(libvlc_media_player_t* player);
    void setPause(libvlc_media_player_t* player);
    void stop(libvlc_media_player_t* player);

private:
    libvlc_instance_t *instanceRight;
    libvlc_media_player_t *mPlayerRight;
    libvlc_media_t *mediaRight;
    libvlc_instance_t *instanceLeft;
    libvlc_media_player_t *mPlayerLeft;
    libvlc_media_t *mediaLeft;
    QMap<QString,QString> deviceMap;
    int doPause = 0;

public slots:
    void handleDeviceChange(QString device);
};


#endif // VLC_H
