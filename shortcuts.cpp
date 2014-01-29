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
    QPushButton *apply = new QPushButton(tr("Reset Defaults"));

    QBoxLayout *hLay = new QHBoxLayout;
    hLay->addStretch(1);
    hLay->addWidget(setNewCut);
    hLay->addWidget(apply);

    QBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(lbld);
    vlay->addWidget(scutTable);
    vlay->addLayout(hLay);

    setLayout(vlay);

    connect(setNewCut,SIGNAL(clicked()),this,SLOT(keySequenceInput()));
    connect(apply,SIGNAL(clicked()),this,SLOT(resetData()));

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
       QStandardItem *item2 = new QStandardItem(QString(tr("Using Defaults")));
       model->setItem(i, 2, item2);
    }

}

void shortcuts::keySequenceInput()
{
     QSettings settings;
    edit = new shortcutEditor(this);
    int row = scutTable->selectionModel()->selection().indexes().first().row();

    edit->exec();

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

void shortcuts::resetData()
{
    defaultData();
    scutTable->setModel(model);
     QSettings settings;

     for(int row=0; row<6;++row){


         QString key = model->item(row,0)->text().toLower();
         key.simplified();
         key.replace(" ", "");
          QString value = model->item(row,1)->text().toLower();

         if(key.contains("show")){

             settings.setValue("shortcut/show_playlist", value);
         }else if(key.contains("play")){

             settings.setValue("shortcut/play_pause", value);
         }else{
             settings.setValue("shortcut/"+ key, value);
         }
        }
 }

void shortcuts::readSequence(QKeySequence keySequence)
{
 seq=keySequence;

}
