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
    void stop();

private slots:
    void mediaStateChanged(QMediaPlayer::State);
    void setPosition(int);
    void positionChanged(qint64);
    void durationChanged(qint64);
    void changeVolume(int);

private:
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    QSlider *seekr, *volSlide;
    QMenuBar *gblMenu;
    QMenu *fileMenu;
    QPushButton *playButton, *stopButton;
    QVideoWidget *videoWidget;


};

#endif // CLEPS_VIDPLAYER_H
