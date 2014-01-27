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

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:
    void open();
    void quit();
    void play();
    void stop();
    void mute();


private slots:
    void changeVolume(int);
    void durationChanged(qint64);
    void mediaStateChanged(QMediaPlayer::State);
    void positionChanged(qint64);
    void setPosition(int);


private:
    QMediaPlayer *playerD;
    QMediaPlaylist *playlist;
    QMenuBar *gblMenu;
    QMenu *fileMenu;
    QListView *plist;
    Cleps_VidPlayer *player;
    QPushButton *playButton, *stopButton, *volLbl;
    QSlider *seekr, *volSlide;

};

#endif // MAINWINDOW_H
