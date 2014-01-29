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

#include "Cleps_MainWindow.h"
#include "cleps_vidplayer.h"
#include "settings_dialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setGeometry(50,50,700,500);

    this->setWindowTitle("Cleps Video Player");

    playerD = new QMediaPlayer;
    playlist = new QMediaPlaylist;
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    playerD->setPlaylist(playlist);
    player = new Cleps_VidPlayer(this);

    playerD->setVideoOutput(player->videoWidget);

    setCentralWidget(player);


    fileMenu = new QMenu(tr("&Media"));
    QAction *opVid = new QAction(tr("&Add to Playlist"), this);
    QAction *quit = new QAction(tr("&Quit"), this);


     connect(opVid, SIGNAL(triggered()), this, SLOT(open()));
     connect(quit, SIGNAL(triggered()), this, SLOT(quit()));

    fileMenu->addAction(opVid);
    fileMenu->addSeparator();
    fileMenu->addAction(quit);

    settingsMenu = new QMenu(tr("Tools"));
    QAction *config = new QAction(tr("Preferences"), this);

    connect(config,SIGNAL(triggered()),this,SLOT(viewSettings()));
    settingsMenu->addAction(config);


    gblMenu = new QMenuBar();
    gblMenu->addMenu(fileMenu);
    gblMenu->addMenu(settingsMenu);
    gblMenu->ensurePolished();

    this->setMenuBar(gblMenu);


    playButton = new QToolButton();
    playButton->setToolTip(tr("Play/Pause"));
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    stopButton= new QToolButton();
    stopButton->setToolTip(tr("Stop"));
    stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

    next = new QToolButton();
    next->setToolTip(tr("Next"));
    next->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));

    previous = new QToolButton();
    previous->setToolTip(tr("Previous"));
    previous->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));


 seekr = new QSlider(Qt::Horizontal);
 seekr->setRange(0,0);

 volSlide = new volumeSlider;
 volSlide->setValue(100);
 volSlide->setToolTip(tr("Volume"));
 volSlide->adjustSize();

     QToolBar *tBard = new QToolBar(tr("View Transport"));

     tBard->addWidget(playButton);
     tBard->addWidget(stopButton);
     tBard->addWidget(previous);
     tBard->addWidget(next);
     tBard->addWidget(seekr);
     tBard->addWidget(volSlide);

     tBard->setFloatable(false);
     tBard->setMovable(false);
     tBard->adjustSize();

     addToolBar(tBard);

     opVid->setShortcut(QKeySequence::Open);
     quit->setShortcut(QKeySequence::Quit);
     config->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
     mte = new QShortcut(this);
     shwList = new QShortcut(this);

       connect(playerD, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
       connect(seekr, SIGNAL(sliderMoved(int)),this, SLOT(setPosition(int)));
       connect(volSlide, SIGNAL(volumeChanged(int)),this, SLOT(changeVolume(int)));
       connect(playerD, SIGNAL(stateChanged(QMediaPlayer::State)),this, SLOT(mediaStateChanged(QMediaPlayer::State)));
       connect(playerD, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
       connect(playerD, SIGNAL(currentMediaChanged(QMediaContent)),this,SLOT(mediaChanged()));

       connect(stopButton, SIGNAL(clicked()),this,SLOT(stop()));
       connect(playButton, SIGNAL(clicked()),this, SLOT(play()));
       connect(next,SIGNAL(clicked()),this,SLOT(nextMedia()));
       connect(previous, SIGNAL(clicked()),this,SLOT(previousMedia()));
       connect(mte,SIGNAL(activated()),this,SLOT(mute()));
       connect(shwList, SIGNAL(activated()),this, SLOT(showPlaylist()));


       viewer = new playlistView(this);
        readSettings();

}

void MainWindow::clear()
{
    playlist->clear();
    plist.clear();
    viewer->setPlaylist(plist);
    stop();
}


void MainWindow::open(){

QString fileName = QFileDialog::getOpenFileName(this, tr("Open Video"), QDir::homePath());

  if (!fileName.isEmpty()){

     playlist->addMedia(QUrl::fromLocalFile((fileName)));

      fileName = fileName.section('/', -1);
     plist.append(fileName);
     viewer->setPlaylist(plist);
  }

}


void MainWindow::changeVolume(int volume){

    playerD->setVolume(volume);
}

void MainWindow::durationChanged(qint64 timed){

    seekr->setRange(0,timed);
}

void MainWindow::mediaChanged()
{

    setWindowTitle(plist.value(playlist->currentIndex())+tr(" - Cleps Video Player"));
}



void MainWindow::mediaStateChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

        break;
    default:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

        break;
    }

}



void MainWindow::mute(){

    switch(playerD->isMuted()){
    case true : playerD->setMuted(false);
                 volSlide->setDisabled(false);
                break;
    case false : playerD->setMuted(true);
                 volSlide->setDisabled(true);
                 break;
    }
}

void MainWindow::nextMedia()
{
    playlist->next();

    setWindowTitle(plist.value(playlist->currentIndex())+tr(" - Cleps Video Player"));
}

void MainWindow::previousMedia()
{
    playlist->previous();
    setWindowTitle(plist.value(playlist->currentIndex())+tr(" - Cleps Video Player"));
}

void MainWindow::showPlaylist()
{
    viewer->show();
    viewer->raise();
    viewer->activateWindow();
}



void MainWindow::play(){

playerD->setVolume(100);

    switch(playerD->state()) {
        case QMediaPlayer::PlayingState:
            playerD->pause();
            break;
        default:
            playerD->play();
            break;
    }
    setWindowTitle(plist.value(playlist->currentIndex())+tr(" - Cleps Video Player"));
}

void MainWindow::playd(QModelIndex index)
{
    playlist->setCurrentIndex(index.row());
    playerD->stop();
    playerD->setPosition(0);
    playerD->setVideoOutput(player->videoWidget);
    play();

}


void MainWindow::stop(){

    playerD->stop();
    playerD->setPosition(0);
    playerD->setVideoOutput(player->videoWidget);
    playlist->setCurrentIndex(0);
    this->setWindowTitle("Cleps Video Player");
}


void MainWindow::setPosition(int position){

   playerD->setPosition(position);

}

void MainWindow::readSettings()
{
     QSettings settings;

     playButton->setShortcut(settings.value("shortcut/play_pause").value<QKeySequence>());
     stopButton->setShortcut(settings.value("shortcut/stop").value<QKeySequence>());
     next->setShortcut(settings.value("shortcut/nextmedia").value<QKeySequence>());
     previous->setShortcut(settings.value("shortcut/previousmedia").value<QKeySequence>());
     mte->setKey((settings.value("shortcut/mute").value<QKeySequence>()));
     shwList->setKey((settings.value("shortcut/show_playlist").value<QKeySequence>()));
}


void MainWindow::viewSettings()
{
    settings_dialog *cfg = new settings_dialog(this);
    cfg->show();
}

void MainWindow::positionChanged(qint64 position){

    seekr->setValue(position);
}

void MainWindow::quit(){

    this->close();
}
