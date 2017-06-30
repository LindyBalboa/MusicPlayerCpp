#ifndef VLC_H
#define VLC_H

#include "vlc/vlc.h"
#include <map>
#include <string>

#include <QMap>
#include <QObject>
#include <QString>

using namespace std;

class VLC : public QObject
{
    Q_OBJECT

    public:
        VLC(QObject *parent = 0);

        void changeTrack(QString path);
        QMap<QString, QString> getDevices();
        void next();
        void previous();
        void setPosition(qint64 currentTime);
        int getLength();
signals:
        bool endReached();
        void lengthChanged(qint64);
        void timeChanged(qint64);
    public slots:
        void handleDeviceChange(QString device);
        void pause();
        void play();
        void setPause();
        void setVolume(qint64 volume);
        void stop();
    private:
        int doPause = 0;
        bool playerIsInit = false;
        QMap<QString,QString> deviceMap;
        libvlc_event_manager_t *eventManager;
        libvlc_instance_t *instance;
        libvlc_media_t *media;
        libvlc_media_player_t *mediaPlayer;

        static void callbacks(const libvlc_event_t *event, void *ptr);
        void registerEvents();
        void updateDeviceMap(QMap<QString, QString> &deviceMap);
};


#endif // VLC_H
