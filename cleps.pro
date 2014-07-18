#-------------------------------------------------
#
# Project created by QtCreator 2014-01-26T02:06:32
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cleps
TEMPLATE = app
target.path = /usr/bin/

INSTALLS += target

SOURCES += main.cpp\
        cleps_vidplayer.cpp \
    Cleps_MainWindow.cpp \
    volumeslider.cpp \
    playlistview.cpp \
    settings_dialog.cpp \
    shortcuteditor.cpp \
    shortcuts.cpp \
    behaviour_settings.cpp \
    subtitleprovider.cpp \
    bookmarkmanager.cpp

HEADERS  += cleps_vidplayer.h \
    Cleps_MainWindow.h \
    volumeslider.h \
    playlistview.h \
    settings_dialog.h \
    shortcuteditor.h \
    shortcuts.h \
    behaviour_settings.h \
    subtitleprovider.h \
    bookmarkmanager.h

TRANSLATIONS =

RESOURCES += \
    icons.qrc


QMAKE_CXXFLAGS += -std=c++11

OTHER_FILES += \
    cleps-video-player.desktop

