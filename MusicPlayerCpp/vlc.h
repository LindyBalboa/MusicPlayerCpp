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
    void next();
    void previous();
    void changeTrack(QString path);
    void setPosition(qint64 currentTime);
private:
    libvlc_instance_t *instance;
    libvlc_media_player_t *mediaPlayer;
    libvlc_media_t *media;
    libvlc_event_manager_t *eventManager;
    QMap<QString,QString> deviceMap;
    void updateDeviceMap(QMap<QString, QString> &deviceMap);
    int doPause = 0;
    bool playerIsInit = false;
    void registerEvents();
    static void callbacks(const libvlc_event_t *event, void *ptr);
public slots:
    void play();
    void pause();
    void setPause();
    void stop();
    void setVolume(qint64 volume);
    void handleDeviceChange(QString device);
signals:
    timeChanged(qint64);
    lengthChanged(qint64);
    endReached();
};


#endif // VLC_H
