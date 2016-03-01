#include "vlc.h"
#include "vlc/vlc.h"

VLC::VLC(QObject *parent) : QObject(parent)
{
    //
    //VLC Set up begins here
    //
    const char * const vlc_args[] = {
             // "--verbose=2", //be much more verbose then normal for debugging purpose
               };
    instanceRight = libvlc_new(0,vlc_args);
    mediaRight = libvlc_media_new_location(instanceRight, "file:///C:/test.m4a");
    mPlayerRight = libvlc_media_player_new_from_media(mediaRight);
    //play(mPlayerRight);

    instanceLeft = libvlc_new(0,vlc_args);
    mediaLeft = libvlc_media_new_location(instanceLeft, "file:///C:/test.mp3");
    mPlayerLeft = libvlc_media_player_new_from_media(mediaLeft);

    libvlc_audio_output_device_t* devices;
    devices = libvlc_audio_output_device_list_get(instanceLeft,"directx");
    while(devices!=NULL)
    {
       deviceMap[devices->psz_description] = devices->psz_device ;
       devices = devices->p_next;

    }
}


void VLC::handleDeviceChange(QString device) //For selecting playback device from Audio menu
{
    QChar whichPlayer = device[0];
    device = device.remove(0,1);
    device = deviceMap[device];
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
QMap<QString, QString> VLC::getDevices()
{
    return deviceMap;
}

void VLC::play(libvlc_media_player_t* player)
{
   libvlc_media_player_play(player);
   doPause = 1;
}

void VLC::pause(libvlc_media_player_t* player)
{
   libvlc_media_player_pause(player);
   doPause = 0;
}

void VLC::setPause(libvlc_media_player_t* player)
{
   libvlc_media_player_set_pause(player, doPause);
   if (doPause==0) {doPause=1;} else {doPause=0;}
}

void VLC::stop(libvlc_media_player_t* player)
{
   libvlc_media_player_stop(player);
}
