#include "volumeslider.h"
#include <QBoxLayout>
#include <QWidgetAction>
#include <QStyle>

volumeSlider::volumeSlider(QWidget *parent) :
    QToolButton(parent)
{

   this->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
   this->setPopupMode(QToolButton::InstantPopup);

    QWidget *popup = new QWidget(this);

    volSlider = new QSlider(Qt::Horizontal);
    volSlider->setRange(0,100);
    connect(volSlider, SIGNAL(valueChanged(int)), this, SIGNAL(volumeChanged(int)));


    QBoxLayout *popupLayout = new QVBoxLayout(popup);
    popupLayout->addWidget(volSlider);


    QWidgetAction *action = new QWidgetAction(this);
    action->setDefaultWidget(popup);

    volMen = new QMenu(this);
    volMen->addAction(action);
    setMenu(volMen);

}

void volumeSlider::setValue(int val){

    volSlider->setValue(val);
}
