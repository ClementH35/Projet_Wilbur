#include "recup_gyroscope.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Recup_Gyroscope w;
    w.show();

    return a.exec();
}
