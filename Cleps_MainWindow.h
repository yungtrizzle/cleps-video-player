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

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);


signals:

public slots:
    void clear();
    void open();
    void quit();
    void play();
    void playd(QModelIndex index);
    void stop();
    void mute();
    void nextMedia();
    void previousMedia();
    void showPlaylist();
    void loadMedia(QString media);


private slots:
    void changeVolume(int);
    void durationChanged(qint64);
    void mediaChanged();
    void mediaStateChanged(QMediaPlayer::State);
    void positionChanged(qint64);
    void setPosition(int);
    void setRandom();
    void setRepeat();
    void setRepeatOne();
    void setSequential();
    void setupTray();
   // void showOsd(QString text);
    void readSettings();
    void toggleHideWindow(QSystemTrayIcon::ActivationReason reason);
    void viewSettings();


private:
    QMediaPlayer *playerD;
    QMediaPlaylist *playlist;
    QMenuBar *gblMenu;
    QMenu *fileMenu, *settingsMenu, *ctxt, *playlistModeMenu;
    QStringList plist;
    Cleps_VidPlayer *player;
    QToolButton *playButton, *stopButton, *next, *previous;
    QSlider *seekr;
    volumeSlider *volSlide;
    playlistView *viewer;
    QShortcut *mte, *shwList;
    QSystemTrayIcon *cleps;
    QString *notifyFlag;

};

#endif // MAINWINDOW_H
