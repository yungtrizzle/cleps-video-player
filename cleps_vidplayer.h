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

#ifndef CLEPS_VIDPLAYER_H
#define CLEPS_VIDPLAYER_H

#include <QtWidgets>
#include <QMediaPlayer>
#include <QMediaPlaylist>



class Cleps_VidPlayer : public QWidget
{
    Q_OBJECT

public:
    Cleps_VidPlayer(QWidget *parent = 0);
    ~Cleps_VidPlayer();
    QVideoWidget *videoWidget;

public slots:
   void context(QPoint pos);

private slots:
   void exitFullScreen();
   void fullScreen();
   void mouseDoubleClickEvent(QMouseEvent *event);
   void contextMenuEvent(QContextMenuEvent *event);

private:


};

#endif // CLEPS_VIDPLAYER_H
