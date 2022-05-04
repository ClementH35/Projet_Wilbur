#include "gps.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //cout << "GPS" << endl;
    GPS w;
    qDebug() << "avant";
    w.show();
    qDebug() << "apres";
    return a.exec();

}

