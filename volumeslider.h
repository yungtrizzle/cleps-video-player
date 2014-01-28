#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QToolButton>
#include <QSlider>
#include <QMenu>


class volumeSlider : public QToolButton
{
    Q_OBJECT
public:
    explicit volumeSlider(QWidget *parent = 0);
    void setValue(int val);

signals:
   void volumeChanged(int volume);

public slots:

private:

  QSlider *volSlider;
  QMenu *volMen;

};

#endif // VOLUMESLIDER_H
