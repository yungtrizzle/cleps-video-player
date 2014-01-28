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

#include "playlistview.h"
#include <QBoxLayout>
#include <QMenu>

playlistView::playlistView(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Playlist"));
    setFixedSize(400,350);
    mediaList = new QListView();
    mediaList->setUniformItemSizes(true);
    mediaModel = new QStringListModel(mediaList);
    QBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(mediaList);

    setLayout(layout);

    connect(mediaList, SIGNAL(doubleClicked(QModelIndex)),this->parent(),SLOT(playd(QModelIndex)));
    connect(mediaList,SIGNAL(activated(QModelIndex)), this->parent(), SLOT(playd(QModelIndex)));
}

void playlistView::setPlaylist(QStringList list)
{
    mediaModel->setStringList(list);
    mediaList->setModel(mediaModel);

}

void playlistView::contextMenuEvent(QContextMenuEvent *event)
{

    QMenu menu(this);
    menu.addAction(tr("Add Media"), this->parent(), SLOT(open()));
    menu.addAction(tr("Clear Playlist"), this->parent(), SLOT(clear()));
    menu.exec(event->globalPos());

}



