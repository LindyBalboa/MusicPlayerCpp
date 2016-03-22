#include "vlc.h" //header for this file
#include "vlc/vlc.h" //header that includes all the libvlc.dll required headers
#include <QDebug>

VLC::VLC(QObject *parent) : QObject(parent)
{
    //
    //VLC Set up begins here
    //
    const char * const vlc_args[] = {
             // "--verbose=2", //be much more verbose then normal for debugging purpose
               };
    instance = libvlc_new(0,vlc_args);
    mediaPlayer = libvlc_media_player_new(instance);
    libvlc_audio_set_volume(mediaPlayer, 50);
    updateDeviceMap(deviceMap);
    registerEvents();
}

void VLC::play()
{
   libvlc_media_player_play(mediaPlayer);
   doPause = 1;
   playerIsInit = true;
}
void VLC::pause()
{
   libvlc_media_player_pause(mediaPlayer);
   doPause = 0;
}
void VLC::setPause()
{
   if (playerIsInit==true)
   {
       libvlc_media_player_set_pause(mediaPlayer, doPause);
       if (doPause==0) {doPause=1;} else {doPause=0;}
   }else
   {
       libvlc_media_player_play(mediaPlayer);
       playerIsInit = true;
       doPause=1;
   }
}
void VLC::stop()
{
    libvlc_media_player_stop(mediaPlayer);
    doPause = 0;
    playerIsInit = false;
}
void VLC::next()
{

}
void VLC::previous()
{

}
void VLC::changeTrack(QString path)
{
    stop();
    play();
    //qDebug() << libvlc_media_player_get_state(mediaPlayer);
    libvlc_media_t *newMedia = libvlc_media_new_location(instance, ("file:///" + path.toStdString()).c_str() ) ;
    libvlc_media_player_set_media(mediaPlayer, newMedia);
    play();
}
void VLC::setVolume(qint64 volume)
{
   libvlc_audio_set_volume(mediaPlayer, volume);
}
void VLC::setPosition(qint64 currentTime)
{
    libvlc_media_player_set_time(mediaPlayer, currentTime);
}

QMap<QString, QString> VLC::getDevices()
{
    return deviceMap;
}

void VLC::registerEvents()
{
    eventManager = libvlc_media_player_event_manager(mediaPlayer);
    // Register the callback
    libvlc_event_attach( eventManager, libvlc_MediaPlayerSnapshotTaken,   callbacks, this );
    libvlc_event_attach( eventManager, libvlc_MediaPlayerTimeChanged,     callbacks, this );
    libvlc_event_attach( eventManager, libvlc_MediaPlayerPlaying,         callbacks, this );
    libvlc_event_attach( eventManager, libvlc_MediaPlayerPaused,          callbacks, this );
    libvlc_event_attach( eventManager, libvlc_MediaPlayerStopped,         callbacks, this );
    libvlc_event_attach( eventManager, libvlc_MediaPlayerEndReached,      callbacks, this );
    libvlc_event_attach( eventManager, libvlc_MediaPlayerPositionChanged, callbacks, this );
    libvlc_event_attach( eventManager, libvlc_MediaPlayerLengthChanged,   callbacks, this );
    libvlc_event_attach( eventManager, libvlc_MediaPlayerEncounteredError,callbacks, this );
    libvlc_event_attach( eventManager, libvlc_MediaPlayerPausableChanged, callbacks, this );
    libvlc_event_attach( eventManager, libvlc_MediaPlayerSeekableChanged, callbacks, this );
}
void VLC::callbacks(const libvlc_event_t* event, void* ptr)
{
    VLC* self = reinterpret_cast<VLC*>( ptr );
    switch ( event->type )
    {
    case libvlc_MediaPlayerPlaying:
        //qDebug() << "Media player playing";
        //self->emit playing();
        break;
    case libvlc_MediaPlayerPaused:
        //qDebug() << "Media player paused";
        //self->emit paused();
        break;
    case libvlc_MediaPlayerStopped:
        //qDebug() << "Media player stopped";
        //self->emit stopped();
        break;
    case libvlc_MediaPlayerEndReached:
        self->emit endReached();
        break;
    case libvlc_MediaPlayerTimeChanged:
        //qDebug() << "time change";
        self->emit timeChanged( event->u.media_player_time_changed.new_time );
        break;
    case libvlc_MediaPlayerPositionChanged:
        //qDebug() << self << "position changed : " << event->u.media_player_position_changed.new_position;
        //self->emit positionChanged( event->u.media_player_position_changed.new_position );
        break;
    case libvlc_MediaPlayerLengthChanged:
        self->emit lengthChanged( event->u.media_player_length_changed.new_length );
        break;
    case libvlc_MediaPlayerSnapshotTaken:
        //self->emit snapshotTaken( event->u.media_player_snapshot_taken.psz_filename );
        break;
    case libvlc_MediaPlayerEncounteredError:
        //qDebug() << '[' << (void*)self << "] libvlc_MediaPlayerEncounteredError received."
         //       << "This is not looking good...";
        //self->emit errorEncountered();
        break;
    case libvlc_MediaPlayerSeekableChanged:
        // TODO: Later change it to an event that corresponds volume change, when this thing gets fixed in libvlc
        //self->emit volumeChanged();
        break;
    case libvlc_MediaPlayerPausableChanged:
    case libvlc_MediaPlayerTitleChanged:
    case libvlc_MediaPlayerNothingSpecial:
    case libvlc_MediaPlayerOpening:
    case libvlc_MediaPlayerBuffering:
    case libvlc_MediaPlayerForward:
    case libvlc_MediaPlayerBackward:
    default:
//        qDebug() << "Unknown mediaPlayerEvent: " << event->type;
        break;
    }
}
void VLC::updateDeviceMap(QMap<QString, QString> &deviceMap)
{
    deviceMap.clear();
    libvlc_audio_output_device_t* devices = libvlc_audio_output_device_list_get(instance,"directx");
    while(devices!=NULL)
    {
       deviceMap[devices->psz_description] = devices->psz_device ;
       devices = devices->p_next;
    }
}
void VLC::handleDeviceChange(QString device) //For selecting playback device from Audio menu
{
    //QChar whichPlayer = device[0];
    //device = device.remove(0,1);
    device = deviceMap[device];
    float pos = libvlc_media_player_get_position(mediaPlayer);
    libvlc_media_player_stop(mediaPlayer);
    libvlc_audio_output_device_set(mediaPlayer, "directx", device.toStdString().c_str());
    libvlc_media_player_play(mediaPlayer);
    libvlc_media_player_set_position(mediaPlayer,pos);
}

int VLC::getLength()
{
    qDebug() << "DUR" << libvlc_media_get_duration(libvlc_media_player_get_media(mediaPlayer));
    return libvlc_media_get_duration(libvlc_media_player_get_media(mediaPlayer) );
}
