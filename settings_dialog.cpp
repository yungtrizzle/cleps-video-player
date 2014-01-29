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

#include "settings_dialog.h"
#include "shortcuts.h"

settings_dialog::settings_dialog(QWidget *parent) :
    QDialog(parent)
{

    setMinimumSize(550,400);
    setFixedWidth(550);
        contents = new QListWidget;
        contents->setViewMode(QListView::IconMode);
        contents->setIconSize(QSize(96, 84));
        contents->setMovement(QListView::Static);
        contents->setMaximumWidth(128);
        contents->setSpacing(12);

        tabs = new QStackedWidget;
        tabs->addWidget(new shortcuts);


        QListWidgetItem *shortcutsButton = new QListWidgetItem(contents);
           shortcutsButton->setIcon(QIcon(":/icons/keyboard.png"));
           shortcutsButton->setText(tr("Shortcuts"));
           shortcutsButton->setTextAlignment(Qt::AlignHCenter);
           shortcutsButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        connect(contents,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this, SLOT(changeTab(QListWidgetItem*,QListWidgetItem*)));

        QBoxLayout *horizontalLayout = new QHBoxLayout;
        horizontalLayout->addWidget(contents);
        horizontalLayout->addWidget(tabs,1);

        QBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addLayout(horizontalLayout);
        setLayout(mainLayout);

        setWindowTitle(tr("Settings"));
}

void settings_dialog::changeTab(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
            current = previous;

        tabs->setCurrentIndex(contents->row(current));
}
