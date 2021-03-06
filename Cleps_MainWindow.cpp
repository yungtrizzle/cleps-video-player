﻿
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
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusMetaType>
#include <QVideoWidget>
#include <QMimeType>
#include <QHoverEvent>
#include <QDataStream>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setGeometry(50,50,1024,768);
    this->setWindowTitle("Cleps Video Player");
    this->setWindowIcon(QIcon(":/icons/cleps.png"));

    playerD = new QMediaPlayer;
    playlist = new QMediaPlaylist;

    playerD->setPlaylist(playlist);
    player = new Cleps_VidPlayer(this);

    playerD->setVideoOutput(player->videoWidget);
    playerD->setVolume(100);
    playlist->setPlaybackMode(QMediaPlaylist::Sequential);

    setCentralWidget(player);
     
    setAcceptDrops(true);

    fileMenu = new QMenu(tr("&Media"));
    opVid = new QAction(tr("&Add to Playlist"), this);
    saveList = new QAction(tr("Save Playlist"),this);
    QAction *quit = new QAction(tr("&Quit"), this);
    no_histry = new QAction(tr("Incognito Mode"),this);
    no_histry->setCheckable(true);

    fileMenu->addAction(opVid);
    fileMenu->addAction(saveList);
    fileMenu->addSeparator();
    fileMenu->addAction(no_histry);
    recent = fileMenu->addMenu(tr("Recent Files"));
    fileMenu->addSeparator();
    fileMenu->addAction(quit);


    for(int i=0; i<5; i++){

        recentF[i] = new QAction(this);
        recentF[i]->setVisible(false);
        connect(recentF[i], SIGNAL(triggered()),this, SLOT(openRecentFiles()));

        recent->addAction(recentF[i]);
    }


    cleaR = new QAction(tr("Clear Menu"), this);
    recent->addSeparator();
    recent->addAction(cleaR);

    settingsMenu = new QMenu(tr("Tools"));
    config = new QAction(tr("Preferences"), this);
    mdlist = new QAction(tr("View Playlist"), this);
    bookmk = new QAction(tr("Manage Bookmarks"), this);

    mode1 = new QAction(tr("Sequential"), this);
    mode1->setCheckable(true);
    mode2 = new QAction(tr("Repeat"), this);
    mode2->setCheckable(true);
    mode3 = new QAction(tr("Repeat One"), this);
    mode3->setCheckable(true);
    mode4 = new QAction(tr("Random"), this);
    mode4->setCheckable(true);
       
    
    modeGrp = new QActionGroup(this);
    modeGrp->addAction(mode1);
    modeGrp->addAction(mode2);
    modeGrp->addAction(mode3);
    modeGrp->addAction(mode4);
    mode1->setChecked(true);
       
    playlistModeMenu = settingsMenu->addMenu(tr("Playlist Mode"));
    settingsMenu->addAction(mdlist);
    settingsMenu->addAction(bookmk);
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
 seekr->setMouseTracking(true);
 seekr->setAttribute(Qt::WA_Hover, true);
 seekr->installEventFilter(this);


 volSlide = new volumeSlider;
 volSlide->setRange(0,100);
 volSlide->setValue(100);
 volSlide->setToolTip(tr("Volume"));
 volSlide->adjustSize();

     QToolBar *tBard = new QToolBar(tr("View Transport"));

     tBard->addWidget(playButton);
     tBard->addWidget(previous);
     tBard->addWidget(stopButton);
     tBard->addWidget(next);
     tBard->addWidget(seekr);
     tBard->addWidget(volSlide);

     tBard->setFloatable(false);
     tBard->setMovable(false);
     tBard->adjustSize();

     
     addToolBar(tBard);
     

     ffwd = new QToolButton();
     ffwd->setToolTip(tr("Fast Foward, click twice to play at normal rate"));
     ffwd->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
     
     rwind = new QToolButton();
     rwind->setToolTip(tr("Reverse, click twice to play at normal rate"));
     rwind->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
     
     
     bookmark = new QToolButton();
     bookmark->setToolTip(tr("Create Bookmark"));
     bookmark->setIcon(QIcon(":/icons/goldstar.png"));

     mediaTimeLbl = new QLabel("--:--/--:--");
     mediaTimeLbl->setToolTip(tr("Media Duration"));

     sbar = new QStatusBar();
     sbar->addPermanentWidget(rwind);
     sbar->addPermanentWidget(ffwd);
     sbar->addPermanentWidget(bookmark);
     sbar->addPermanentWidget(new QLabel);
     sbar->addPermanentWidget(mediaTimeLbl);
     sbar->addPermanentWidget(new QLabel);

     setStatusBar(sbar);

     opVid->setShortcut(QKeySequence::Open);
     opVid->setShortcutContext(Qt::ApplicationShortcut);
     quit->setShortcut(QKeySequence::Quit);
     quit->setShortcutContext(Qt::ApplicationShortcut);

     config->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
     config->setShortcutContext(Qt::ApplicationShortcut);
     mte = new QShortcut(this);
     mte->setContext(Qt::ApplicationShortcut);
   
     bookmk->setShortcut(Qt::CTRL+Qt::Key_B);
     bookmk->setShortcutContext(Qt::ApplicationShortcut);
     mdlist->setShortcut(Qt::CTRL+Qt::Key_L);
     mdlist->setShortcutContext(Qt::ApplicationShortcut);
     saveList->setShortcut(QKeySequence::Save);
     saveList->setShortcutContext(Qt::ApplicationShortcut);

     ply = new QShortcut(this);
     ply->setContext(Qt::ApplicationShortcut);
     stp = new QShortcut(this);
     stp->setContext(Qt::ApplicationShortcut);
     nxt = new QShortcut(this);
     nxt->setContext(Qt::ApplicationShortcut);
     prv = new QShortcut(this);
     prv->setContext(Qt::ApplicationShortcut);

     connect(opVid, SIGNAL(triggered()), this, SLOT(open()));
     connect(quit, SIGNAL(triggered()), this, SLOT(quit()));
     connect(config,SIGNAL(triggered()),this,SLOT(viewSettings()));
     connect(mdlist, SIGNAL(triggered()), this,SLOT(showPlaylist()));
     connect(saveList,SIGNAL(triggered()),this,SLOT(savePlayList()));
     connect(cleaR, SIGNAL(triggered()), this, SLOT(clearRecent()));
     connect(bookmk, SIGNAL(triggered()),this,SLOT(showManager()));
     connect(no_histry,SIGNAL(toggled(bool)),this,SLOT(noHistory(bool)));

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
       connect(ply, SIGNAL(activated()),this,SLOT(play()));
       connect(stp, SIGNAL(activated()),this, SLOT(stop()));
       connect(nxt, SIGNAL(activated()),this,SLOT(nextMedia()));
       connect(prv,SIGNAL(activated()),this,SLOT(previousMedia()));

      connect(mode1,SIGNAL(triggered()),this,SLOT(setSequential()));
      connect(mode2,SIGNAL(triggered()),this,SLOT(setRepeat()));
      connect(mode3,SIGNAL(triggered()),this,SLOT(setRepeatOne()));
      connect(mode4,SIGNAL(triggered()),this,SLOT(setRandom()));

      
      connect(ffwd,SIGNAL(clicked()),this,SLOT(fastForward()));
      connect(rwind,SIGNAL(clicked()),this,SLOT(rewind()));
      connect(bookmark,SIGNAL(clicked()),this,SLOT(bmarks()));
     
       viewer = new playlistView(this);
    

       connect(viewer,SIGNAL(removeIndex(QList<int>)), this, SLOT(removeMedia(QList<int>)));
       connect(viewer,SIGNAL(swapIndex(int,int)),this,SLOT(swap(int, int)));

       trayVisible = false; notifyFlag=false; runbckgd = false; hasSubs = false;
       quitPlistEnd = false;
       readSettings();
       openBookmarks();

       cfg = new settings_dialog(this);
       connect(cfg, SIGNAL(accepted()),this, SLOT(readSettings()));

        manager = new bookmarkManager(this, (getBookmark()));
        connect(manager, SIGNAL(createMark()),this,SLOT(bmarks()));
        connect(manager,SIGNAL(deleteMark(QUrl)),this,SLOT(deleteBmark(QUrl)));
        connect(this,SIGNAL(newBookmark()),this,SLOT(reloadBmarks()));

}

void MainWindow::changeEvent(QEvent *event)
{

    if (event->type() == QEvent::WindowStateChange && (isMinimized()||this->isVisible())){


        QUrl uri = playerD->currentMedia().canonicalResource().url();
        QMimeType mime = db.mimeTypeForUrl(uri);

        if(mime.name().contains("video")){
             play();
     }

    }

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


void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{

    if(event->mimeData()->hasFormat("text/uri-list")){
        event->acceptProposedAction();

    }


}

void MainWindow::dropEvent(QDropEvent *event)
{

    QList<QUrl> urls = event->mimeData()->urls();
        if (urls.isEmpty())
            return;

        for(const QUrl &file:urls){

            if(!file.isEmpty()){
                loadMedia(file.toLocalFile());
            }
        }

}

void MainWindow::hideEvent(QHideEvent *event)
{

    QUrl uri = playerD->currentMedia().canonicalResource().url();
    QMimeType mime = db.mimeTypeForUrl(uri);

   if(mime.name().contains("video")){
        play();
}
   event->accept();
}


void MainWindow::throwFilenotFound(QString str)
{
  QString errstrn = tr("Cannot find File ")+str;
  
  err.showMessage(errstrn);
  qDebug()<<errstrn;
  
}


QMap<QUrl, qint64> *MainWindow::getBookmark()
{
    QMap<QUrl, qint64> *mks;
    mks = &bookmarks;

    return mks;

}


void MainWindow::clear()
{
    playlist->clear();
    plist.clear();
    viewer->setPlaylist(plist);
    stop();
}

void MainWindow::clearRecent()
{
    QSettings settings;

  settings.setValue("Recent Files",QStringList());
  readSettings();
}


void MainWindow::open(){

QStringList fileName = QFileDialog::getOpenFileNames(this, tr("Open Media"), QDir::homePath());

  if (!fileName.isEmpty()){


      for(const QString &str: fileName){
	
	if(QFile::exists(str)){
	  
	  if(str.contains("m3u",Qt::CaseInsensitive)){
	  loadPlayList(str);
	  fileName.removeOne(str);
	  }else{
       
         playlist->addMedia(QUrl::fromLocalFile((str)));
	  
	  
	 if(historyFlag){
          if(rcntCache.size()>5){
               rcntCache.removeLast();
         }
             rcntCache.prepend(str);
	 }
          QString str2 = str.section('/', -1);
          plist.append(str2);
        }
      }
     else{
      throwFilenotFound(str); 
     }
      } 
    }
     
     viewer->setPlaylist(plist);
}


void MainWindow::changeVolume(int volume){

    playerD->setVolume(volume);
}

void MainWindow::deleteBmark(QUrl uri)
{

    QFile file(QDir::homePath()+"/.cleps.bmk");

        file.open(QIODevice::WriteOnly);

       QDataStream rwout(&file);
       rwout.setVersion(QDataStream::Qt_5_3);
       bookmarks.remove(uri);
       rwout<<bookmarks;

       file.close();

       openBookmarks();

    emit newBookmark();
}

void MainWindow::durationChanged(qint64 timed){

    QString drtin = "--:--/" + millisToHHMMSS(timed);
    durtion = timed;
    seekr->setRange(0,timed);
    mediaTimeLbl->setText(drtin);

}

void MainWindow::insertMark()
{

    QUrl url = playerD->currentMedia().canonicalResource().url();
    qint64 position = playerD->position();

    if(!playerD->currentMedia().isNull()){

    bookmarks.insert(url, position);
    }
}

void MainWindow::mediaChanged()
{
    QSettings settings;

    hasSubs = false;
    if(playlist->currentMedia().isNull()){
        setWindowTitle("Cleps Video Player");
    }else{
    setWindowTitle(plist.value(playlist->currentIndex())+" - Cleps Video Player");

     if(notifyFlag){
          showNativeNotify();
}
}

    while(rcntCache.size()>5){

        rcntCache.removeLast();
    }
   rcntCache.removeDuplicates();
    settings.setValue("Recent Files",rcntCache);

    rcntCache = settings.value("Recent Files").toStringList();

    if(!rcntCache.isEmpty()){

        for(const QString str:rcntCache){
            int i = rcntCache.indexOf(str);
            if(!str.isEmpty()){
            recentF[i]->setData(str);
            recentF[i]->setText(str.section('/', -1));
            recentF[i]->setVisible(true);
        }
    }
}

}

void MainWindow::mediaStateChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    case QMediaPlayer::StoppedState:
        if(quitPlistEnd && playlist->nextIndex() == 0){

            quit();
        }

    default:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

        break;
    }

}

void MainWindow::openRecentFiles()
{

    QAction *action = qobject_cast<QAction *>(sender());
        if (action)
            loadMedia(action->data().toString());

        play();
}

void MainWindow::mute(){

    if(playerD->isMuted()){
    playerD->setMuted(false);
    volSlide->changeIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    volSlide->setDisabled(false);
                }

    else{
          playerD->setMuted(true);
	  volSlide->changeIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
          volSlide->setDisabled(true);
    }
    }


void MainWindow::nextMedia()
{
    playlist->next();

    if(playlist->currentMedia().isNull()){
        setWindowTitle("Cleps Video Player");
    }else{

    setWindowTitle(plist.value(playlist->currentIndex())+" - Cleps Video Player");
}}

void MainWindow::previousMedia()
{
    playlist->previous();

    if(playlist->currentMedia().isNull()){
        setWindowTitle("Cleps Video Player");
    }else{
        setWindowTitle(plist.value(playlist->currentIndex())+" - Cleps Video Player");
    }}

void MainWindow::removeMedia(QList<int> list)
{

    for(const int idx: list){

        plist.removeAt(idx);
        playlist->removeMedia(idx);
    }
            viewer->setPlaylist(plist);

}

void MainWindow::savePlayList()
{

  QString listName = QFileDialog::getSaveFileName(this,tr("Save Playlist"),QDir::homePath(),tr("Playlist Files(*.m3u)"));
   playlist->save(QUrl::fromUserInput(listName),"m3u");
  qDebug() << playlist->error();

}

void MainWindow::showPlaylist()
{
    if(!viewer->isVisible()){

    viewer->show();
    viewer->raise();
    viewer->activateWindow();

}else{
        viewer->hide();

    }
}

void MainWindow::swap(int old, int newd)
{

    if(old >= 0 && old < plist.size()){
        if(newd>=0 && newd != plist.size()){

            plist.move(old, newd);
            QMediaContent md= playlist->media(old);
            playlist->removeMedia(old);
            playlist->insertMedia(newd, md);
            viewer->setPlaylist(plist);
        }
    }

}

void MainWindow::loadMedia(QString media)
{
 

    if (!media.isEmpty()&& QFile::exists(media)){

      playlist->addMedia(QUrl::fromLocalFile((media)));
      

      if(historyFlag){
       if(rcntCache.size()>5){
            rcntCache.removeLast();
      }
        rcntCache.prepend(media);
      }
      
        media = media.section('/', -1);
       plist.append(media);
       viewer->setPlaylist(plist);
    }else{
    
      throwFilenotFound(media);
    }
}

void MainWindow::loadPlayList(QString str){
   
        QFile f(str);
        if(!f.open(QIODevice::ReadOnly|QIODevice::Text)){

            qDebug() << "Unable to open file " << f.fileName();
	    err.showMessage("Unable to open file "+f.fileName());
           return;
        }
            QTextStream in(&f);

            while(!in.atEnd()){
                QString line = in.readLine();
		QUrl md(line);
               loadMedia(md.toLocalFile());

            }
            f.close();

}

void MainWindow::fastForward()
{
   qreal rate = playerD->playbackRate();
  
   if(rate == 1.0){
     playerD->setPlaybackRate(2.0);
  }else {
    playerD->setPlaybackRate(1.0);
 }
}

void MainWindow::rewind()
{

 qreal rate = playerD->playbackRate();
  
   if(rate == 1.0){
     playerD->setPlaybackRate(-1.0);
  }else {
    playerD->setPlaybackRate(1.0);
 }  
  
}

void MainWindow::noHistory(bool hsi)
{

  historyFlag = !hsi;
  
  if(hsi){
  
    no_histry->setChecked(true);
    bookmark->setEnabled(false);
    manager->toggleBookmarks(false);
  sbar->showMessage(tr("Incognito Mode"));
  
  }else{

   no_histry->setChecked(false);
   bookmark->setEnabled(true);
   manager->toggleBookmarks(true);
   sbar->clearMessage(); 
  }
  
}


bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
   if(obj == seekr){

       if(ev->type() == QEvent::HoverEnter||ev->type() == QEvent::HoverMove){

           QHoverEvent *hover = static_cast<QHoverEvent*>(ev);

           QPoint hovPt = hover->pos();


           float posRatio = hovPt.x() / (float )seekr->size().width();
           qint64 sliderRange = seekr->maximum() - seekr->minimum();
           qint64 sliderPosUnderMouse = seekr->minimum() + sliderRange * posRatio;

            if(sliderPosUnderMouse > 0){

           seekr->setToolTip(millisToHHMMSS(sliderPosUnderMouse));
          }

            return true;
       }else{
           return false;
       }

   }else{
       return QMainWindow::eventFilter(obj, ev);
   }



}


void MainWindow::aboutIt()
{
    QString title = tr("About Cleps");
        QString text = "Cleps Video Player is simply a video player, no more, no less. \nyungtrizzle (C)2014";

        QMessageBox::about(this, title, text);

}
void MainWindow::bmarks()
{


    QFile file(QDir::homePath()+"/.cleps.bmk");

        file.open(QIODevice::WriteOnly);

       QDataStream rwout(&file);
       rwout.setVersion(QDataStream::Qt_5_3);

         insertMark();
         rwout<<bookmarks;

         file.close();

         openBookmarks();
         emit newBookmark();
}


void MainWindow::play(){

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
    setWindowTitle(plist.value(playlist->currentIndex())+" - Cleps Video Player");
    }
    
}

void MainWindow::playBookmark(QModelIndex index)
{

    QUrl ur = bookmarks.keys().at(index.row());
    loadMedia(ur.toLocalFile());

     int inde = playlist->mediaCount()-1;

    playlist->setCurrentIndex(inde);
     playerD->stop();
     playerD->stop();
    playerD->setVideoOutput(player->videoWidget);
    play();
    playerD->setPosition(bookmarks.value(ur));
}

void MainWindow::playd(QModelIndex index)
{
    playlist->setCurrentIndex(index.row());
    playerD->stop();
    playerD->setPosition(0);
    playerD->setVideoOutput(player->videoWidget);
    play();

    //qDebug()<<playerD->mediaStatus() << "\n" << playerD->error();
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
    medName = new QAction(tr("No Media"),0);
    
    ctxt = new QMenu(this);
    ctxt->addAction(medName);
    ctxt->addSeparator();
    ctxt->addAction(tr("Play/Pause"),this, SLOT(play()));
    ctxt->addAction(tr("Previous"),this, SLOT(previousMedia()));
    ctxt->addAction(tr("Next"),this, SLOT(nextMedia()));
    ctxt->addAction(tr("Stop"),this, SLOT(stop()));
    ctxt->addSeparator();
    ctxt->addAction(tr("Quit"),this, SLOT(quit()));

    connect(cleps,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(toggleHideWindow(QSystemTrayIcon::ActivationReason)));

    cleps->setContextMenu(ctxt);
    cleps->setToolTip("Cleps Video Player");
    cleps->show();
    }
}

void MainWindow::showManager()
{
    manager->show();
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

                   // qDebug() << reply.error();
    
    medName->setText(plist.value(playlist->currentIndex()));
    
    
}


void MainWindow::readSettings()
{
     QSettings settings;

     ply->setKey(settings.value("shortcut/play_pause").value<QKeySequence>());
     stp->setKey(settings.value("shortcut/stop").value<QKeySequence>());
     nxt->setKey(settings.value("shortcut/nextmedia").value<QKeySequence>());
     prv->setKey(settings.value("shortcut/previousmedia").value<QKeySequence>());
     mte->setKey((settings.value("shortcut/mute").value<QKeySequence>()));

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

         if(settings.value("system/playlist_quit").toInt() != 0){
             quitPlistEnd = true;
         }

         rcntCache = settings.value("Recent Files").toStringList();

         if(!rcntCache.isEmpty()){

             for(const QString str:rcntCache){
                 int i = rcntCache.indexOf(str);
                 if(!str.isEmpty()){
                 recentF[i]->setData(str);
                 recentF[i]->setText(str.section('/', -1));
                 recentF[i]->setVisible(true);
             }
         }
}
}

void MainWindow::toggleHideWindow(QSystemTrayIcon::ActivationReason reason)
{

    if(reason == QSystemTrayIcon::Trigger){

        if(this->isVisible()){
        this->setHidden(true);

         }else{
            this->show();

        }

    }

}


void MainWindow::viewSettings()
{
    cfg->show();
}

QString MainWindow::millisToHHMMSS(qint64 millis)
{
     qint64 seconds = (millis / 1000) % 60;
     qint64 minutes = (millis / (1000 * 60)) % 60;
     qint64 hours = millis / (1000 * 60 * 60);
     QString mins, secs;


     if(minutes < 10){
         mins = "0"+QString::number(minutes);
     }else{
         mins = QString::number(minutes);
     }

      if (seconds < 10){
          secs = "0"+QString::number(seconds);
      }else{
          secs = QString::number(seconds);
      }

     return (QString::number(hours)+":"+mins+":"+secs);

}

void MainWindow::openBookmarks()
{

    QFile file(QDir::homePath()+"/.cleps.bmk");

    if(file.exists()){

        file.open(QIODevice::ReadOnly);

       QDataStream rwout(&file);
       rwout.setVersion(QDataStream::Qt_5_3);

         rwout >> bookmarks;

        // qDebug()<<bookmarks.keys() << bookmarks.values();

         file.close();

}

}

void MainWindow::positionChanged(qint64 position){

    seekr->setValue(position);
    
    QString tim = millisToHHMMSS(position)+"/"+millisToHHMMSS(durtion);
    
   mediaTimeLbl->setText(tim);
   
}

void MainWindow::reloadBmarks()
{
    manager->reload();
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


    QSettings settings;

    while(rcntCache.size()>5){

        rcntCache.removeLast();
    }
    rcntCache.removeDuplicates();
    settings.setValue("Recent Files",rcntCache);

    qApp->exit(0);

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
