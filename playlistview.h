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

signals:

public slots:

private:

    QListView *mediaList;
    QStringListModel *mediaModel;

};

#endif // PLAYLISTVIEW_H
