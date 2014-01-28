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

#include "cleps_vidplayer.h"
#include <QVideoWidget>


Cleps_VidPlayer::Cleps_VidPlayer(QWidget *parent)
    : QWidget(parent)

{
    videoWidget = new QVideoWidget;

    QBoxLayout *mreLayout = new QHBoxLayout;
    mreLayout->setMargin(0);
    mreLayout->setSpacing(0);

    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(videoWidget);
    layout->addLayout(mreLayout);

       setLayout(layout);


       QShortcut *fll = new QShortcut(QKeySequence::FullScreen, videoWidget);
       connect(fll,SIGNAL(activated()),this,SLOT(fullScreen()));

}


void Cleps_VidPlayer::fullScreen(){

    if(videoWidget->isFullScreen()){
        videoWidget->setFullScreen(false);
    }else {
        videoWidget->setFullScreen(true);
    }
}

void Cleps_VidPlayer::mouseDoubleClickEvent(QMouseEvent *event)
{
   videoWidget->setFullScreen(!isFullScreen());
   event->accept();
}

void Cleps_VidPlayer::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(tr("Open Playlist"), this->parent(), SLOT(showPlaylist()));
//add menu items as needed

    menu.exec(event->globalPos());

}

Cleps_VidPlayer::~Cleps_VidPlayer()
{

}
