#-------------------------------------------------
#
# Project created by QtCreator 2016-02-20T17:15:26
#
#-------------------------------------------------

QT       += core gui multimedia sql
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MusicPlayerCpp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vlc.cpp \
    playlistwidget.cpp \
    playerwidget.cpp \
    sliderclass.cpp \
    filesystemscanner.cpp \
    librarywidgets.cpp \
    optionswidget.cpp \
    searchdialog.cpp

HEADERS  += mainwindow.h \
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
    sliderclass.h \
    filesystemscanner.h \
    librarywidgets.h \
    optionswidget.h \
    searchdialog.h \
    globals.h

FORMS    +=

LIBS +=  -lvlc
LIBS +=  -ltag

OTHER_FILES += database.db

INCLUDEPATH += /home/conner/Source_Builds/MusicPlayerCpp/MusicPlayerCpp/VLC \
#INCLUDEPATH += "/home/conner/Source_Builds/MusicPlayerCpp/MusicPlayerCpp/taglib"

DISTFILES += \
    libvlc.so \
    libvlc.so \
    libvlccore.so \
    device_list \
    libmysql.so

