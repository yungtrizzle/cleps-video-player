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


#ifndef BOOKMARKMANAGER_H
#define BOOKMARKMANAGER_H

#include <QWidget>
#include <QtWidgets>

class bookmarkManager : public QDialog
{
    Q_OBJECT
public:
    explicit bookmarkManager(QWidget *parent = 0, QMap<QUrl, qint64> *mks = 0);

signals:
    void createMark();
    void deleteMark(QUrl uri);

public slots:
    void addBookmark();
    void removeBookmark();
    void clearDb();
    void reload();

private :
     QMap<QUrl, qint64> *bmarks;
     QTableView *table;
     QStandardItemModel *model;

     void fillModel();
     QString toHHMMSS(qint64 millis);
};

#endif // BOOKMARKMANAGER_H
