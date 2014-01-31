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


#include "behaviour_settings.h"

behaviour_settings::behaviour_settings(QWidget *parent) :
    QWidget(parent)
{

  QLabel *lbl = new QLabel(tr("<b>Under the Hood</b>"));
  trCon = new QCheckBox(tr("Tray Icon           *May Require Restart"));
  notifi = new QCheckBox(tr("Allow Notifications"));
  runBhd = new QCheckBox(tr("Keep Running in the Background"));
  QPushButton * apli = new QPushButton(tr("Apply"));

 QBoxLayout *hlay = new QHBoxLayout;
 hlay->addStretch(1);
 hlay->addWidget(apli);


 QBoxLayout *vlay = new QVBoxLayout;
 vlay->setMargin(0);
 vlay->setSpacing(0);
 vlay->addWidget(lbl);
 vlay->addWidget(trCon);
 vlay->addWidget(runBhd);
 vlay->addWidget(notifi);
 vlay->insertStretch(12);
 vlay->addLayout(hlay);

 setLayout(vlay);
 flags = new QMap<QString, QVariant>;

 connect(notifi,SIGNAL(stateChanged(int)),this,SLOT(ntfyType(int)));
 connect(trCon, SIGNAL(stateChanged(int)),this,SLOT(tray(int)));
 connect(runBhd, SIGNAL(stateChanged(int)),this,SLOT(bckgd(int)));
 connect(apli,SIGNAL(clicked()),this,SLOT(writeAll()));

 readSettings();


}

void behaviour_settings::bckgd(int stea)
{
    flags->insert("Run_Background", stea);
}

void behaviour_settings::ntfyType(int state){
    flags->insert("Notify", state);
}

void behaviour_settings::tray(int state){

    if(state == 0){

        runBhd->setCheckState(Qt::Unchecked);

    }

    flags->insert("Tray", state);
}


void behaviour_settings::writeAll(){

    QSettings settings;

    settings.setValue("system/notify", flags->value("Notify",0));
    settings.setValue("system/tray", flags->value("Tray",0));
    settings.setValue("system/run_background", flags->value("Run_Background",0));

}

void behaviour_settings::readSettings(){

    QSettings settings;

    flags->insert("Notify", settings.value("system/notify"));
    flags->insert("Tray", settings.value("system/tray"));
    flags->insert("Run_Background", settings.value("system/run_background"));

    if(flags->value("Notify").toInt() != 0){

    notifi->setCheckState(Qt::Checked);
    }

    if(flags->value("Tray").toInt() !=  0){
    trCon->setCheckState(Qt::Checked);
}

    if(settings.value("system/run_background").toInt() != 0){

        runBhd->setCheckState(Qt::Checked);

    }


 }
