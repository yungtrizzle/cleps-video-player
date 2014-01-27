#include "cleps_vidplayer.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString locale = QLocale::system().name();

    QTranslator translator;
    translator.load(QString("Cleps_") + locale);
    a.installTranslator(&translator);

    Cleps_VidPlayer w;
    w.show();

    return a.exec();
}
