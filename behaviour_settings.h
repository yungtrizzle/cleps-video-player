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


#ifndef BEHAVIOUR_SETTINGS_H
#define BEHAVIOUR_SETTINGS_H

#include <QWidget>
#include <QtWidgets>

class behaviour_settings : public QWidget
{
    Q_OBJECT
public:
    explicit behaviour_settings(QWidget *parent = 0);

signals:

public slots:
    void ntfyType(int state);
    void tray(int state);
    void writeAll();
private slots:
    void readSettings();

private:
    QGroupBox *bhve;
    QRadioButton *osdNoyify, *nativeNotify;
    QCheckBox *trCon, *notifi;
    QMap<QString,QVariant> *flags;
};

#endif // BEHAVIOUR_SETTINGS_H
