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
#include <QSettings>
#include <QShortcut>
#include <QPushButton>
#include <QMimeData>

playlistView::playlistView(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Playlist"));
    setFixedSize(400,450);
    setAcceptDrops(true);
    mediaList = new QListView();
    mediaList->setUniformItemSizes(true);
    mediaList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mediaList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mediaModel = new QStringListModel(mediaList);

    QPushButton *upI = new QPushButton;
    upI->setToolTip(tr("Move Up"));
    upI->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));


    QPushButton *downI = new QPushButton;
    downI->setToolTip(tr("Move Down"));
    downI->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));


    QPushButton *delI = new QPushButton;
    delI->setToolTip(tr("Delete"));
    delI->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));


    QPushButton *opn = new QPushButton;
    opn->setToolTip(tr("Open media"));
    opn->setIcon(style()->standardIcon(QStyle::SP_DirIcon));


    QBoxLayout *hlay = new QHBoxLayout;
    hlay->addStretch(1);
    hlay->addWidget(opn);
    hlay->addWidget(delI);
    hlay->addWidget(upI);
    hlay->addWidget(downI);

    QBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(mediaList);
    layout->addLayout(hlay);

    setLayout(layout);

   QShortcut *deleted = new QShortcut(this);
   deleted->setKey(QKeySequence(Qt::Key_Delete));

    connect(mediaList, SIGNAL(doubleClicked(QModelIndex)),this->parent(),SLOT(playd(QModelIndex)));
    connect(mediaList,SIGNAL(activated(QModelIndex)), this->parent(), SLOT(playd(QModelIndex)));
    connect(delI, SIGNAL(clicked()), this, SLOT(remove()));
    connect(deleted,SIGNAL(activated()), this, SLOT(remove()));
    connect(opn, SIGNAL(clicked()),this->parent(),SLOT(open()));
    connect(upI,SIGNAL(clicked()),this,SLOT(mveUp()));
    connect(downI,SIGNAL(clicked()),this,SLOT(mveDown()));
    connect(this,SIGNAL(addMedia(QString)),this->parentWidget(),SLOT(loadMedia(QString)));

}

void playlistView::remove()
{
    QModelIndexList deleteList = mediaList->selectionModel()->selection().indexes();

    QList<int> dlIndex;

    if(!deleteList.isEmpty()){

    for(const QModelIndex &idx: deleteList){

        dlIndex.append(idx.row());
    }

 emit removeIndex(dlIndex);
}

}

void playlistView::mveUp(){

    QModelIndexList deleteList = mediaList->selectionModel()->selection().indexes();

    int row, nwrow;

    if(!deleteList.isEmpty()){
     row = deleteList.first().row();
    nwrow = row--;


  emit swapIndex(row, nwrow);


}

}

void playlistView::mveDown()
{

    QModelIndexList deleteList = mediaList->selectionModel()->selection().indexes();

    int row, nwrow;

    if(!deleteList.isEmpty()){
     row = deleteList.first().row();
    nwrow = row++;

    emit swapIndex(row,nwrow);
}
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
    menu.addAction(tr("Remove Selected"), this, SLOT(remove()));
    menu.addSeparator();
    menu.addAction(tr("Play/Pause"),this->parent(), SLOT(play()));
    menu.addAction(tr("Previous"),this->parent(), SLOT(previousMedia()));
    menu.addAction(tr("Next"),this->parent(), SLOT(nextMedia()));
    menu.addAction(tr("Stop"),this->parent(), SLOT(stop()));
    menu.addSeparator();
    menu.addAction(tr("Hide Playlist"), this->parent(), SLOT(showPlaylist()));
    menu.addAction(tr("Clear Playlist"), this->parent(), SLOT(clear()));
    menu.addAction(tr("Quit"),this->parent(), SLOT(quit()));

    menu.exec(event->globalPos());

}

void playlistView::closeEvent(QCloseEvent *event)
{

    hide();
    event->ignore();

}

void playlistView::dragEnterEvent(QDragEnterEvent *event)
{

    if(event->mimeData()->hasFormat("text/uri-list")){
        event->acceptProposedAction();

    }
}

void playlistView::dropEvent(QDropEvent *event)
{

    QList<QUrl> urls = event->mimeData()->urls();
        if (urls.isEmpty())
            return;

        for(const QUrl &file:urls){

            if(!file.isEmpty()){
             emit addMedia(file.toLocalFile());
        }
}
}


