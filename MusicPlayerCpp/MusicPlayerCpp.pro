#-------------------------------------------------
#
# Project created by QtCreator 2016-02-20T17:15:26
#
#-------------------------------------------------

QT       += core gui multimedia sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MusicPlayerCpp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    librarywidget.cpp \
    vlc.cpp \
    playlistwidget.cpp \
    playerwidget.cpp \
    sliderclass.cpp

HEADERS  += mainwindow.h \
        librarywidget.h \
    vlc/vlc.h \
    vlc/deprecated.h \
    vlc/libvlc.h \
    vlc/libvlc_events.h \
    vlc/libvlc_media.h \
    vlc/libvlc_media_discoverer.h \
    vlc/libvlc_media_library.h \
    vlc/libvlc_media_list.h \
    vlc/libvlc_media_list_player.h \
    vlc/libvlc_media_player.h \
    vlc/libvlc_structures.h \
    vlc/libvlc_vlm.h \
    vlc.h \
    playlistwidget.h \
    playerwidget.h \
    sliderclass.h

FORMS    +=

LIBS += -LC:\Users\Conner\Documents\MusicPlayerCpp\MusicPlayerCpp -llibvlc
LIBS += -LC:\Users\Conner\Documents\MusicPlayerCpp\MusicPlayerCpp -llibmysql

OTHER_FILES += database.db

INCLUDEPATH += C:\Users\Conner\Documents\MusicPlayerCpp\MusicPlayerCpp\VLC \

DISTFILES += \
    libvlc.lib \
    libvlc.dll \
    libvlccore.dll \
    device_list \
    libmysql.dll

