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


#ifndef SHORTCUTS_H
#define SHORTCUTS_H

#include <QWidget>
#include <QtWidgets>
#include "shortcuteditor.h"


class shortcuts : public QWidget
{
    Q_OBJECT
public:
    explicit shortcuts(QWidget *parent = 0);
    void defaultData();
    QStandardItemModel *model;
    QStringList data;
    QTableView *scutTable;
    QKeySequence seq;
    shortcutEditor *edit;

signals:

public slots:
    void keySequenceInput();
    void resetData();
    void readSequence(QKeySequence keySequence);
};

#endif // SHORTCUTS_H
