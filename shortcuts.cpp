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


#include "shortcuts.h"

shortcuts::shortcuts(QWidget *parent) :
    QWidget(parent)
{
    model = new QStandardItemModel(6,3);
    model->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Default Shortcut") << tr("Active Shortcut"));
    data << tr("Play/Pause") << tr("Space") << tr("Next Media") << tr("N")  << tr("Mute") << tr("M");
    data << tr("Stop")<< tr("S") << tr("Previous Media") << tr("P") << tr("Show PlayList") << tr("Ctrl+L");

    defaultData();
    scutTable = new QTableView(this);
    scutTable->setModel(model);
    scutTable->setSelectionMode(QAbstractItemView::SingleSelection);
    scutTable->verticalHeader()->setVisible(false);
    scutTable->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    scutTable->horizontalHeader()->setStretchLastSection(true);
    scutTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    scutTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    scutTable->setAlternatingRowColors(true);
    scutTable->setShowGrid(false);

    QLabel *lbld = new QLabel(tr("<b>Media Shortcuts</b>"));

    QPushButton *setNewCut = new QPushButton(tr("Change ShortCut"));
    QPushButton *reset = new QPushButton(tr("Reset"));

    QBoxLayout *hLay = new QHBoxLayout;
    hLay->addStretch(1);
    hLay->addWidget(setNewCut);
    hLay->addWidget(reset);

    QBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(lbld);
    vlay->addWidget(scutTable);
    vlay->addLayout(hLay);

    setLayout(vlay);

    connect(setNewCut,SIGNAL(clicked()),this,SLOT(keySequenceInput()));
    connect(reset,SIGNAL(clicked()),this,SLOT(resetData()));

}
void shortcuts::defaultData()
{
     QStringListIterator javaStyleIterator(data);

    for(int row=0; row<6;++row){
        for (int column = 0; column<2; ++column){

            QStandardItem *item = new QStandardItem(javaStyleIterator.next());
                  model->setItem(row, column, item);
        }
    }

    for(int i = 0; i<6;++i){
       QStandardItem *item2 = new QStandardItem(QString(tr(" ")));
       model->setItem(i, 2, item2);
    }

}

void shortcuts::keySequenceInput()
{
     QSettings settings;
    edit = new shortcutEditor(this);
   int row= -1;
   QModelIndexList lst =   scutTable->selectionModel()->selection().indexes();


        if(lst.isEmpty()){
      return;

    }else{
            row = lst.first().row();

    edit->exec();

    if(!seq.isEmpty()){

    model->item(row, 2)->setText(seq.toString());
    scutTable->setModel(model);

    QString key = model->item(row,0)->text().toLower();
    key.simplified();
    key.replace(" ", "");

   if(key.contains("show")){

       settings.setValue("shortcut/show_playlist", seq);
   }else if(key.contains("play")){

       settings.setValue("shortcut/play_pause", seq);
   }else{
       settings.setValue("shortcut/"+ key, seq);

   }
   }
    }
}

void shortcuts::resetData()
{
    defaultData();
    scutTable->setModel(model);
     QSettings settings;

     for(int row=0; row<6;++row){


         QString key = model->item(row,0)->text().toLower();
         key = key.simplified();
        key = key.replace(" ", "");
          QString value = model->item(row,1)->text().toLower();

         if(key.contains("show")){

             settings.setValue("shortcut/show_playlist", value);
         }else if(key.contains("play")){

             settings.setValue("shortcut/play_pause", value);
         }else{
             settings.setValue("shortcut/"+ key, value);
         }
        }

     this->parentWidget()->parentWidget()->close();

 }

void shortcuts::readSequence(QKeySequence keySequence)
{

 seq=keySequence;

}
