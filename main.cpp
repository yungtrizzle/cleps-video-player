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

#include "cleps_vidplayer.h"
#include "Cleps_MainWindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName("Cleps");
    a.setApplicationName("Cleps Video Player");
    a.setApplicationVersion("0.21.1");

    QString locale = QLocale::system().name();

    QTranslator translator;
    translator.load(QString("Cleps_") + locale);
    a.installTranslator(&translator);

    QCommandLineParser parser;
    parser.setApplicationDescription(QApplication::tr("A small video player."));
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addPositionalArgument("filename", QApplication::tr("A media file to play."));

    parser.process(a);
     const QStringList args = parser.positionalArguments();

      MainWindow w;

     for(const QString &str: args){
         w.loadMedia(str);
     }

    w.show();
    w.play();

    return a.exec();
}
