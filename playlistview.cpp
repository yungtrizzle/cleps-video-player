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



