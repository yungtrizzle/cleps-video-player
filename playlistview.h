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

#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include <QDialog>
#include <QListView>
#include <QStringListModel>
#include <QCloseEvent>

class playlistView : public QDialog
{
    Q_OBJECT
public:
    explicit playlistView(QWidget *parent = 0);

    void setPlaylist(QStringList list);
    void contextMenuEvent(QContextMenuEvent *event);
    void closeEvent(QCloseEvent *event);

signals:
    void removeIndex(QList<int>);
    void swapIndex(int,int);

public slots:
     void remove();
     void mveUp();
     void mveDown();

private:

    QListView *mediaList;
    QStringListModel *mediaModel;

};

#endif // PLAYLISTVIEW_H
