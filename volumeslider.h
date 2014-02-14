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
    void setRange(int start, int end);

signals:
   void volumeChanged(int volume);

public slots:

private:

  QSlider *volSlider;
  QMenu *volMen;

};

#endif // VOLUMESLIDER_H
