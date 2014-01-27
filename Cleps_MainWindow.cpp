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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setGeometry(50,50,700,500);

    this->setWindowTitle("Cleps Video Player");

    playerD = new QMediaPlayer;
    playlist = new QMediaPlaylist;
    playerD->setPlaylist(playlist);
    player = new Cleps_VidPlayer(this);

    playerD->setVideoOutput(player->videoWidget);

    setCentralWidget(player);


    fileMenu = new QMenu(tr("&Media"));
    QAction *opVid = new QAction(tr("&Add to Playlist"), this);
    QAction *quit = new QAction(tr("&Quit"), this);


    opVid->setShortcut(QKeySequence::Open);
    quit->setShortcut(QKeySequence::Quit);

     connect(opVid, SIGNAL(triggered()), this, SLOT(open()));
     connect(quit, SIGNAL(triggered()), this, SLOT(quit()));

    fileMenu->addAction(opVid);
    fileMenu->addSeparator();
    fileMenu->addAction(quit);


    gblMenu = new QMenuBar();
    gblMenu->addMenu(fileMenu);
    gblMenu->ensurePolished();

    this->setMenuBar(gblMenu);


    playButton = new QPushButton();
    playButton->setFlat(true);
    playButton->setToolTip(tr("Play/Pause"));
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    stopButton= new QPushButton();
    stopButton->setFlat(true);
    stopButton->setToolTip(tr("Stop"));
    stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

    volLbl = new QPushButton();
    volLbl->setFlat(true);
    volLbl->setToolTip(tr("Mute"));
    volLbl->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

 seekr = new QSlider(Qt::Horizontal);
 seekr->setRange(0,0);



 volSlide = new QSlider(Qt::Horizontal);
 volSlide->setRange(0,100);
 volSlide->setValue(100);
 volSlide->setToolTip(tr("Volume"));
 volSlide->adjustSize();

     QToolBar *tBard = new QToolBar;
     QToolBar *skr = new QToolBar;
     QToolBar *vol = new QToolBar;
     tBard->addWidget(playButton);
     tBard->addWidget(stopButton);

     tBard->setFloatable(false);
     tBard->setMovable(false);
     tBard->adjustSize();

     skr->addWidget(seekr);
     skr->setFloatable(false);
     skr->setMovable(false);
     skr->adjustSize();

     vol->addWidget(volLbl);
     vol->addWidget(volSlide);
     vol->setFloatable(false);
     vol->setMovable(false);
     vol->adjustSize();

     addToolBar(vol);
     addToolBar(tBard);
     addToolBar(skr);

     playButton->setShortcut(QKeySequence(Qt::Key_Space));
     stopButton->setShortcut(QKeySequence(Qt::Key_S));
     volLbl->setShortcut(QKeySequence(Qt::Key_M));

    plist = new QListView;

      QDockWidget *plistWidget = new QDockWidget(this);
      plistWidget->setAllowedAreas(Qt::RightDockWidgetArea);
      plistWidget->setWidget(plist);
      addDockWidget(Qt::RightDockWidgetArea, plistWidget);
    /* plistWidget->setFixedWidth(250);
      plistWidget->setFixedHeight(500);
 */



       connect(playerD, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
       connect(seekr, SIGNAL(sliderMoved(int)),this, SLOT(setPosition(int)));
       connect(volSlide, SIGNAL(valueChanged(int)),this, SLOT(changeVolume(int)));
       connect(playerD, SIGNAL(stateChanged(QMediaPlayer::State)),this, SLOT(mediaStateChanged(QMediaPlayer::State)));
       connect(playerD, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));

       connect(stopButton, SIGNAL(clicked()),this,SLOT(stop()));
       connect(playButton, SIGNAL(clicked()),this, SLOT(play()));
       connect(volLbl,SIGNAL(clicked()),this,SLOT(mute()));


}


void MainWindow::open(){

QString fileName = QFileDialog::getOpenFileName(this, tr("Open Video"), QDir::homePath());

  if (!fileName.isEmpty()){

     playlist->addMedia(QUrl::fromLocalFile((fileName)));
  }

}


void MainWindow::changeVolume(int volume){

    playerD->setVolume(volume);
}

void MainWindow::durationChanged(qint64 timed){

    seekr->setRange(0,timed);
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
}


void MainWindow::stop(){

    playerD->stop();
    playerD->setPosition(0);
    playerD->setVideoOutput(player->videoWidget);
    playlist->setCurrentIndex(0);
}


void MainWindow::setPosition(int position){

   playerD->setPosition(position);

}

void MainWindow::positionChanged(qint64 position){

    seekr->setValue(position);
}

void MainWindow::quit(){

    this->close();
}
