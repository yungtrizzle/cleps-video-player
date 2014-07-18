/******************************************************************************
*                                                                             *
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


#include "bookmarkmanager.h"

bookmarkManager::bookmarkManager(QWidget *parent, QMap<QUrl, qint64> *mks) :
    QDialog(parent)
{


    setFixedSize(650, 400);
   setWindowTitle(tr("Bookmarks"));
   bmarks = mks;

   model = new QStandardItemModel();

   table = new QTableView(this);

   table->setModel(model);
   table->setSelectionMode(QAbstractItemView::SingleSelection);
   table->verticalHeader()->setVisible(false);
   table->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
   table->horizontalHeader()->setStretchLastSection(true);
   table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
   table->setEditTriggers(QAbstractItemView::NoEditTriggers);
   table->setAlternatingRowColors(true);
   table->setShowGrid(false);

    fillModel();

    QPushButton *create = new QPushButton(tr("Create"));
    QPushButton *deleted = new QPushButton(tr("Delete"));
    QPushButton *cler = new QPushButton(tr("Clear"));

    QBoxLayout *hlay = new QHBoxLayout;
    QBoxLayout *vlay = new QVBoxLayout;


    vlay->addWidget(create);
    vlay->addWidget(deleted);
    vlay->addStretch(1);
    vlay->addWidget(cler);

     hlay->addLayout(vlay);
     hlay->addWidget(table);


     setLayout(hlay);

     connect(create, SIGNAL(clicked()),this,SLOT(addBookmark()));
     connect(deleted, SIGNAL(clicked()),this,SLOT(removeBookmark()));
     connect(cler, SIGNAL(clicked()),this,SLOT(clearDb()));
     connect(table,SIGNAL(doubleClicked(QModelIndex)), this->parent(),SLOT(playBookmark(QModelIndex)));

}

void bookmarkManager::addBookmark()
{
   emit createMark();

}

void bookmarkManager::removeBookmark()
{

  int row = -1; QUrl uri;

  QModelIndexList lst = table->selectionModel()->selection().indexes();


  if(lst.isEmpty()){
        return;

    }else{
      row = lst.first().row();

      uri = QUrl::fromLocalFile(model->item(row,0)->text());
      //qDebug()<< uri;
      model->removeRow(row);

  }

    emit deleteMark(uri);


}

void bookmarkManager::clearDb()
{
    QFile::remove(QDir::homePath()+"/.cleps.bmk");
    model->clear();
    bmarks->clear();

}

void bookmarkManager::reload()
{
    fillModel();
    table->setModel(model);
}

void bookmarkManager::fillModel()
{   int a =0;
     model->setHorizontalHeaderLabels(QStringList() << tr("Description") << tr("Time"));
    QMap<QUrl, qint64>::const_iterator i;
        for(i = bmarks->constBegin();  i != bmarks->constEnd(); ++i){

               QStandardItem *item = new QStandardItem(i.key().toLocalFile());

               QStandardItem *item2 = new QStandardItem(toHHMMSS(i.value()));

            model->setItem(a, 0, item);
            model->setItem(a,1, item2);
            a++;
        }
        table->resizeColumnsToContents();
}

QString bookmarkManager::toHHMMSS(qint64 millis)
{
    qint64 seconds = (millis / 1000) % 60;
    qint64 minutes = (millis / (1000 * 60)) % 60;
    qint64 hours = millis / (1000 * 60 * 60);
    QString mins, secs;


    if(minutes < 10){
        mins = "0"+QString::number(minutes);
    }else{
        mins = QString::number(minutes);
    }

     if (seconds < 10){
         secs = "0"+QString::number(seconds);
     }else{
         secs = QString::number(seconds);
     }

    return (QString::number(hours)+":"+mins+":"+secs);


}
