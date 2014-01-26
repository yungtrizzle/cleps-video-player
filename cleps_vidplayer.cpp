#include "cleps_vidplayer.h"
#include <QVideoWidget>
#include <QVideoSurfaceFormat>

Cleps_VidPlayer::Cleps_VidPlayer(QWidget *parent)
    : QWidget(parent)


{
    pState=2;
    player = new QMediaPlayer;//(0, QMediaPlayer::VideoSurface);
    playlist = new QMediaPlaylist;
    player->setPlaylist(playlist);

    videoWidget = new QVideoWidget;
    fileMenu = new QMenu(tr("&File"));
    QAction *opVid = new QAction(tr("&Open Video"), this);
    QAction *quit = new QAction(tr("&Quit"), this);

    opVid->setShortcut(QKeySequence::Open);
    quit->setShortcut(QKeySequence::Quit);

     connect(opVid, SIGNAL(triggered()), this, SLOT(open()));
     connect(quit, SIGNAL(triggered()), this, SLOT(quit()));

    fileMenu->addAction(opVid);
    fileMenu->addAction(quit);

    gblMenu = new QMenuBar();
    gblMenu->addMenu(fileMenu);

       playButton = new QPushButton;
       playButton->setEnabled(false);
       playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

       pause = new QPushButton;
       pause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));


      connect(playButton, SIGNAL(clicked()),this, SLOT(play()));


    seekr = new QSlider(Qt::Horizontal);
    seekr->setRange(0,0);

    connect(seekr, SIGNAL(sliderMoved(int)),this, SLOT(setPosition(int)));


       errorLabel = new QLabel;
       errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);


    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(seekr);

    QBoxLayout *layout = new QVBoxLayout;
       layout->addWidget(gblMenu);
       layout->addWidget(videoWidget);
       layout->addLayout(controlLayout);
       layout->addWidget(errorLabel);

       setLayout(layout);
       player->setVideoOutput(videoWidget);

       connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),this, SLOT(mediaStateChanged(QMediaPlayer::State)));
       connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
       connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
      // connect(&player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(handleError()));

}


/*****************Public Slots*************/

void Cleps_VidPlayer::open(){


QString fileName = QFileDialog::getOpenFileName(this, tr("Open Video"), QDir::homePath());

  if (!fileName.isEmpty()){

      //player->setMedia(QUrl::fromLocalFile((fileName)));
      playlist->addMedia(QUrl::fromLocalFile((fileName)));

  }

  playButton->setEnabled(true);

}

void Cleps_VidPlayer::play(){


    switch(player->state()) {
        case QMediaPlayer::PlayingState:
            player->pause();
            break;
        default:
            player->play();
            break;

}
}


void Cleps_VidPlayer::quit(){

    this->close();

}


/*****************Private Slots*************/
void Cleps_VidPlayer::mediaStateChanged(QMediaPlayer::State state)
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

void Cleps_VidPlayer::setPosition(int position){


    player->setPosition(position);

}

void Cleps_VidPlayer::positionChanged(qint64 position){

    seekr->setValue(position);
}


void Cleps_VidPlayer::durationChanged(qint64 timed){

    seekr->setRange(0,timed);
}






Cleps_VidPlayer::~Cleps_VidPlayer()
{

}
