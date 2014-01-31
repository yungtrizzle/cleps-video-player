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
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusMetaType>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setGeometry(50,50,700,500);

    this->setWindowTitle("Cleps Video Player");
    this->setWindowIcon(QIcon(":/icons/cleps.png"));

    playerD = new QMediaPlayer;
    playlist = new QMediaPlaylist;

    playerD->setPlaylist(playlist);
    player = new Cleps_VidPlayer(this);

    playerD->setVideoOutput(player->videoWidget);

    setCentralWidget(player);


    fileMenu = new QMenu(tr("&Media"));
    QAction *opVid = new QAction(tr("&Add to Playlist"), this);
    QAction *quit = new QAction(tr("&Quit"), this);

    fileMenu->addAction(opVid);
    fileMenu->addSeparator();
    fileMenu->addAction(quit);

    settingsMenu = new QMenu(tr("Tools"));
    QAction *config = new QAction(tr("Preferences"), this);
    QAction *mdlist = new QAction(tr("View Playlist"), this);


    QAction *mode1 = new QAction(tr("Sequential"), this);
    QAction *mode2 = new QAction(tr("Repeat"), this);
    QAction *mode3 = new QAction(tr("Repeat One"), this);
    QAction *mode4 = new QAction(tr("Random"), this);

    settingsMenu->addAction(mdlist);
    playlistModeMenu = settingsMenu->addMenu(tr("Playlist Mode"));
    settingsMenu->addSeparator();
    settingsMenu->addAction(config);

    playlistModeMenu->addAction(mode1);
    playlistModeMenu->addAction(mode2);
    playlistModeMenu->addAction(mode3);
    playlistModeMenu->addAction(mode4);

    about = new QMenu(tr("About"));
    about->addAction(tr("About Cleps Video Player"), this, SLOT(aboutIt()));


    gblMenu = new QMenuBar();
    gblMenu->addMenu(fileMenu);
    gblMenu->addMenu(settingsMenu);
    gblMenu->addMenu(about);
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
 seekr->setPageStep(1);

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

     connect(opVid, SIGNAL(triggered()), this, SLOT(open()));
     connect(quit, SIGNAL(triggered()), this, SLOT(quit()));
     connect(config,SIGNAL(triggered()),this,SLOT(viewSettings()));
     connect(mdlist, SIGNAL(triggered()), this,SLOT(showPlaylist()));

       connect(playerD, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
       connect(seekr, SIGNAL(sliderMoved(int)),this, SLOT(setPosition(int)));
       connect(seekr, SIGNAL(valueChanged(int)), this, SLOT(seekNewPosition(int)));
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

      connect(mode1,SIGNAL(triggered()),this,SLOT(setSequential()));
      connect(mode2,SIGNAL(triggered()),this,SLOT(setRepeat()));
      connect(mode3,SIGNAL(triggered()),this,SLOT(setRepeatOne()));
      connect(mode4,SIGNAL(triggered()),this,SLOT(setRandom()));

       viewer = new playlistView(this);
       trayVisible = false; notifyFlag=false; runbckgd = false;
       readSettings();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
   if(runbckgd){
       setHidden(true);
       event->ignore();
   }else{

       event->accept();
       close();
   }



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
    if(playlist->currentMedia().isNull()){
        setWindowTitle("Cleps Video Player");
    }else{
    setWindowTitle(plist.value(playlist->currentIndex())+tr(" - Cleps Video Player"));


     if(notifyFlag){
          showNativeNotify();
}
}
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

    if(playlist->currentMedia().isNull()){
        setWindowTitle("Cleps Video Player");
    }else{

    setWindowTitle(plist.value(playlist->currentIndex())+tr(" - Cleps Video Player"));
}}

void MainWindow::previousMedia()
{
    playlist->previous();

    if(playlist->currentMedia().isNull()){
        setWindowTitle("Cleps Video Player");
    }else{
    setWindowTitle(plist.value(playlist->currentIndex())+tr(" - Cleps Video Player"));
}}

void MainWindow::showPlaylist()
{
    viewer->show();
    viewer->raise();
    viewer->activateWindow();
}

void MainWindow::loadMedia(QString media)
{

    if (!media.isEmpty()){

       playlist->addMedia(QUrl::fromLocalFile((media)));

        media = media.section('/', -1);
       plist.append(media);
       viewer->setPlaylist(plist);
    }
}

void MainWindow::aboutIt()
{
    QString title = "About Cleps";
        QString text = "Cleps Video Player is a simply video player, no more, no less. \nyungtrizzle (C)2014";

        QMessageBox::about(this, title, text);

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

    if(playlist->currentMedia().isNull()){
        setWindowTitle("Cleps Video Player");
    }else{
    setWindowTitle(plist.value(playlist->currentIndex())+tr(" - Cleps Video Player"));
    }

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

void MainWindow::setRandom()
{
    playlist->setPlaybackMode(QMediaPlaylist::Random);
}

void MainWindow::setRepeat()
{
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
}

void MainWindow::setRepeatOne()
{
    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

}

void MainWindow::setSequential()
{
    playlist->setPlaybackMode(QMediaPlaylist::Sequential);
}

void MainWindow::setupTray()
{
    if(!trayVisible){

    cleps=new QSystemTrayIcon(QIcon(":/icons/cleps.png"),this);
    ctxt = new QMenu(this);
    ctxt->addAction(tr("Play/Pause"),this, SLOT(play()));
    ctxt->addAction(tr("Previous"),this, SLOT(previousMedia()));
    ctxt->addAction(tr("Next"),this, SLOT(nextMedia()));
    ctxt->addAction(tr("Stop"),this, SLOT(stop()));
    ctxt->addSeparator();
    ctxt->addAction(tr("Quit"),this, SLOT(quit()));

    connect(cleps,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(toggleHideWindow(QSystemTrayIcon::ActivationReason)));

    cleps->setContextMenu(ctxt);
    cleps->show();
    }
}

void MainWindow::showNativeNotify()
{
    //argumenList code from https://github.com/rohieb/StratumsphereTrayIcon

    qDBusRegisterMetaType<QImage>();

    QVariantMap hints;
    hints["image_data"] = QImage(":/icons/cleps.png");
    QList<QVariant> argumentList;
      argumentList << "Cleps"; //app_name
      argumentList << (uint)0; // replace_id
      argumentList << ""; // app_icon
      argumentList << "Cleps Video Player"; // summary
      argumentList << plist.value(playlist->currentIndex()); // body
      argumentList << QStringList(); // actions
      argumentList << hints; // hints
      argumentList << (int)-1;


    QDBusInterface notfy("org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications");
    QDBusReply<int> reply = notfy.callWithArgumentList(QDBus::AutoDetect, "Notify", argumentList);

                    qDebug() << reply.error();
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

     if(settings.value("system/tray").toInt() != 0){

         setupTray();
         trayVisible = true;
     }

     if(settings.value("system/notify").toInt() != 0){

         notifyFlag = true;
     }

         if(settings.value("system/run_background").toInt() != 0){
         runbckgd = true;
     }else{
             runbckgd = false;
         }

}

void MainWindow::toggleHideWindow(QSystemTrayIcon::ActivationReason reason)
{

    if(reason == QSystemTrayIcon::Trigger){

        switch(this->isVisible()){
        case true: this->setHidden(true);
                    break;
        case false: this->show();
                    break;
        }

    }

}


void MainWindow::viewSettings()
{
    settings_dialog *cfg = new settings_dialog(this);
    connect(cfg, SIGNAL(accepted()),this, SLOT(readSettings()));
    cfg->show();
}

void MainWindow::positionChanged(qint64 position){

    seekr->setValue(position);
}

void MainWindow::seekNewPosition(int newPos)
{
    // Make slider to follow the mouse directly and not by pageStep steps
        Qt::MouseButtons btns = QApplication::mouseButtons();
        QPoint localMousePos = seekr->mapFromGlobal(QCursor::pos());
        bool clickOnSlider = (btns & Qt::LeftButton) &&
                             (localMousePos.x() >= 0 && localMousePos.y() >= 0 &&
                              localMousePos.x() < seekr->size().width() &&
                              localMousePos.y() < seekr->size().height());
        if (clickOnSlider)
        {
            // Attention! The following works only for Horizontal, Left-to-right sliders
            float posRatio = localMousePos.x() / (float )seekr->size().width();
            int sliderRange = seekr->maximum() - seekr->minimum();
            int sliderPosUnderMouse = seekr->minimum() + sliderRange * posRatio;
            if (sliderPosUnderMouse != newPos)
            {
                seekr->setValue(sliderPosUnderMouse);
                setPosition(sliderPosUnderMouse);

}
}
}

void MainWindow::quit(){

    this->close();

}



/**
* Automatic marshaling of a QImage for org.freedesktop.Notifications.Notify
*
* This function is from the Clementine project (see
* http://www.clementine-player.org) and licensed under the GNU General Public
* License, version 3 or later.
*
* Copyright 2010, David Sansome <me@davidsansome.com>
*/
QDBusArgument& operator<<(QDBusArgument& arg, const QImage& image) {
  if(image.isNull()) {
    arg.beginStructure();
    arg << 0 << 0 << 0 << false << 0 << 0 << QByteArray();
    arg.endStructure();
    return arg;
  }

  QImage scaled = image.scaledToHeight(100, Qt::SmoothTransformation);
  scaled = scaled.convertToFormat(QImage::Format_ARGB32);

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
  // ABGR -> ARGB
  QImage i = scaled.rgbSwapped();
#else
  // ABGR -> GBAR
  QImage i(scaled.size(), scaled.format());
  for (int y = 0; y < i.height(); ++y) {
    QRgb* p = (QRgb*) scaled.scanLine(y);
    QRgb* q = (QRgb*) i.scanLine(y);
    QRgb* end = p + scaled.width();
    while (p < end) {
      *q = qRgba(qGreen(*p), qBlue(*p), qAlpha(*p), qRed(*p));
      p++;
      q++;
    }
  }
#endif

  arg.beginStructure();
  arg << i.width();
  arg << i.height();
  arg << i.bytesPerLine();
  arg << i.hasAlphaChannel();
  int channels = i.isGrayscale() ? 1 : (i.hasAlphaChannel() ? 4 : 3);
  arg << i.depth() / channels;
  arg << channels;
  arg << QByteArray(reinterpret_cast<const char*>(i.bits()), i.byteCount());
  arg.endStructure();
  return arg;
}

const QDBusArgument& operator>>(const QDBusArgument& arg, QImage&) {
  // This is needed to link but shouldn't be called.
  Q_ASSERT(0);
  return arg;
}
