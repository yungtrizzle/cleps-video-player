/******************************************************************************
*   Cleps Video Player. Simply a media player, no more, no less.              *
*   Copyright (C) 2014  Eshton Robateau <eshtonrob@gmail.com>                 *
*                                                                             *
*    This program is free software: you can redistribute it and/or modify     *
*    it under the terms of the GNU General Public License as published by     *
*    the Free Software Foundation, either version 3 of the License, or        *
*    (at your option) any later version.                                      *
*                                                                             *
*    This program is distributed in the hope that it will be useful,          *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*    GNU General Public License for more details.                             *
*                                                                             *
*    You should have received a copy of the GNU General Public License        *
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
*******************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cleps_vidplayer.h"
#include "volumeslider.h"
#include "playlistview.h"
#include "subtitleprovider.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);


signals:

public slots:
    void addSubs();
    void clear();
    void open();
    void quit();
    void play();
    void playd(QModelIndex index);
    void stop();
    void mute();
    void nextMedia();
    void previousMedia();
    void removeMedia(QList<int> list);
    void showPlaylist();
    void showSub();
    void swap(int old, int newd);
    void loadMedia(QString media);


private slots:
    void aboutIt();
    void changeVolume(int);
    void durationChanged(qint64);
    void mediaChanged();
    void mediaStateChanged(QMediaPlayer::State);
    void positionChanged(qint64);
    void seekNewPosition(int newPos);
    void setPosition(int);
    void setRandom();
    void setRepeat();
    void setRepeatOne();
    void setSequential();
    void setupTray();
    void showNativeNotify();
    void readSettings();
    void toggleHideWindow(QSystemTrayIcon::ActivationReason reason);
    void viewSettings();


private:
    QAction *opVid,*config,*mdlist,*subtitle,*mode1, *mode2, *mode3, *mode4;
    QMediaPlayer *playerD;
    QMediaPlaylist *playlist;
    QMenuBar *gblMenu;
    QMenu *fileMenu, *settingsMenu, *ctxt, *playlistModeMenu, *about;
    QStringList plist;
    Cleps_VidPlayer *player;
    QToolButton *playButton, *stopButton, *next, *previous;
    QSlider *seekr;
    volumeSlider *volSlide;
    playlistView *viewer;
    SubtitleProvider *subs;
    QShortcut *mte, *shwList, *ply, *stp, *nxt,*prv;
    QSystemTrayIcon *cleps;
    QLabel *ovlay;
    bool notifyFlag, trayVisible, runbckgd, hasSubs;

};

#endif // MAINWINDOW_H
