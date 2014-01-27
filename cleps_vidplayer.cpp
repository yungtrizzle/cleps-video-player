#include "cleps_vidplayer.h"
#include <QVideoWidget>

Cleps_VidPlayer::Cleps_VidPlayer(QWidget *parent)
    : QWidget(parent)

{
    this->setGeometry(40,50,400,480);
    this->setWindowTitle("Cleps Video Player");

    player = new QMediaPlayer;
    playlist = new QMediaPlaylist;
    player->setPlaylist(playlist);
    videoWidget = new QVideoWidget;

    QShortcut *fll = new QShortcut(QKeySequence::FullScreen, videoWidget);
    connect(fll,SIGNAL(activated()),this,SLOT(fullScreen()));


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

       playButton = new QPushButton();
       playButton->setFlat(true);
       playButton->setToolTip(tr("Play/Pause"));
       playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
       playButton->setShortcut(QKeySequence(Qt::Key_Space));

       stopButton= new QPushButton();
       stopButton->setFlat(true);
       stopButton->setToolTip(tr("Stop"));
       stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
       stopButton->setShortcut(QKeySequence(Qt::Key_S));


      connect(stopButton, SIGNAL(clicked()),this,SLOT(stop()));
      connect(playButton, SIGNAL(clicked()),this, SLOT(play()));


    seekr = new QSlider(Qt::Horizontal);
    seekr->setRange(0,0);

    volSlide = new QSlider(Qt::Horizontal);
    volSlide->setRange(0,100);
    volSlide->setValue(100);
    volSlide->setToolTip(tr("Volume"));
    volSlide->adjustSize();

    connect(seekr, SIGNAL(sliderMoved(int)),this, SLOT(setPosition(int)));
    connect(volSlide, SIGNAL(valueChanged(int)),this, SLOT(changeVolume(int)));

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(seekr);

    QBoxLayout *mreLayout = new QHBoxLayout;
    mreLayout->setMargin(0);
    mreLayout->addWidget(playButton);
    mreLayout->addWidget(stopButton);
    mreLayout->insertStretch(2,800);
    mreLayout->addWidget(volSlide);

    QBoxLayout *layout = new QVBoxLayout;
       layout->addWidget(gblMenu);
       layout->addWidget(videoWidget);
       layout->addLayout(controlLayout);
       layout->addLayout(mreLayout);


       setLayout(layout);
       player->setVideoOutput(videoWidget);


       connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),this, SLOT(mediaStateChanged(QMediaPlayer::State)));
       connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
       connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));

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

player->setVolume(100);

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

void Cleps_VidPlayer::stop(){

    player->stop();
    player->setPosition(0);
    player->setVideoOutput(videoWidget);
    playlist->setCurrentIndex(0);
}


/*****************Private Slots*************/


void Cleps_VidPlayer::changeVolume(int volume){

    volSlide->setValue(volume);
    player->setVolume(volume);
}

void Cleps_VidPlayer::fullScreen(){

    if(videoWidget->isFullScreen()){
        videoWidget->setFullScreen(false);
    }else {
        videoWidget->setFullScreen(true);
    }
}


void Cleps_VidPlayer::durationChanged(qint64 timed){

    seekr->setRange(0,timed);
}


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




Cleps_VidPlayer::~Cleps_VidPlayer()
{

}
