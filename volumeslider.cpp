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

    volSlider = new QSlider(Qt::Vertical);
    volSlider->setRange(0,100);
    volSlider->setPageStep(1);
    volSlider->setSingleStep(1);
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

void volumeSlider::setRange(int start, int end)
{
    volSlider->setRange(start,end);
}

void volumeSlider::changeIcon(QIcon Icon){
  
  
 this->setIcon(Icon); 
  
}
