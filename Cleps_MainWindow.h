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
#include <QMimeDatabase>
#include <QStatusBar>
#include <QMap>
#include <QErrorMessage>
#include "cleps_vidplayer.h"
#include "volumeslider.h"
#include "playlistview.h"
#include "settings_dialog.h"
#include "bookmarkmanager.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void hideEvent(QHideEvent *event);
    void throwFilenotFound(QString str);
    QMap<QUrl, qint64> * getBookmark();
    bool historyFlag = false;


signals:
    void newBookmark();


public slots:
    void clear();
    void clearRecent();
    void open();
    void quit();
    void play();
    void playBookmark(QModelIndex index);
    void playd(QModelIndex index);
    void stop();
    void mute();
    void nextMedia();
    void noHistory(bool hsi);
    void previousMedia();
    void removeMedia(QList<int> list);
    void savePlayList();
    void showPlaylist();
    void swap(int old, int newd);
    void loadMedia(QString media);
    void loadPlayList(QString str);
    void fastForward();
    void rewind();
   

 protected:
     bool eventFilter(QObject *obj, QEvent *ev);


private slots:
    void aboutIt();
    void bmarks();
    void changeVolume(int);
    void deleteBmark(QUrl uri);
    void durationChanged(qint64);
    void insertMark();
    void mediaChanged();
    void mediaStateChanged(QMediaPlayer::State);
    void openRecentFiles();
    void positionChanged(qint64);
    void reloadBmarks();
    void seekNewPosition(int newPos);
    void setPosition(int);
    void setRandom();
    void setRepeat();
    void setRepeatOne();
    void setSequential();
    void setupTray();
    void showManager();
    void showNativeNotify();
    void readSettings();
    void toggleHideWindow(QSystemTrayIcon::ActivationReason reason);
    void viewSettings();



private:
    QAction *opVid,*config,*mdlist,*medName,*mode1,*mode2,*mode3,*mode4,*saveList,*cleaR;
    QAction *recentF[5], *bookmk, *no_histry;
    QMap<QUrl,qint64> bookmarks;
    QMediaPlayer *playerD;
    QMediaPlaylist *playlist;
    QMenuBar *gblMenu;
    QMenu *fileMenu, *settingsMenu, *ctxt, *playlistModeMenu, *about, *recent;
    QStringList plist, rcntCache;
    Cleps_VidPlayer *player;
    QToolButton *playButton, *stopButton, *next, *previous, *ffwd,*rwind,*bookmark;
    QSlider *seekr;
    volumeSlider *volSlide;
    playlistView *viewer;
    QShortcut *mte, *ply, *stp, *nxt,*prv, *bkmarks;
    QSystemTrayIcon *cleps;
    QStatusBar *sbar;
    QLabel *mediaTimeLbl;
    QMimeDatabase db;
    QErrorMessage err;
    bool notifyFlag, trayVisible, runbckgd, hasSubs, quitPlistEnd;
    settings_dialog *cfg;
    bookmarkManager *manager;
   qint64 durtion;
   QActionGroup *modeGrp;

    QString millisToHHMMSS(qint64 millis);
    void openBookmarks();


};

#endif // MAINWINDOW_H
