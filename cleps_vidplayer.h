#ifndef CLEPS_VIDPLAYER_H
#define CLEPS_VIDPLAYER_H

#include <QtWidgets>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>


class Cleps_VidPlayer : public QWidget
{
    Q_OBJECT

public:
    Cleps_VidPlayer(QWidget *parent = 0);
    ~Cleps_VidPlayer();

public slots:
    void open();
    void quit();
    void play();

private slots:
    void mediaStateChanged(QMediaPlayer::State);
    void setPosition(int);
    void positionChanged(qint64);
    void durationChanged(qint64);


private:
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    QSlider *seekr;
    QMenuBar *gblMenu;
    QMenu *fileMenu;
    QAbstractButton *playButton, *pause;
    QVideoWidget *videoWidget;
    QLabel *errorLabel;
    int pState;
};

#endif // CLEPS_VIDPLAYER_H
