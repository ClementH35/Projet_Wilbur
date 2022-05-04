#ifndef GPS_H
#define GPS_H

#include <QMainWindow>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <istream>
#include <QtSql/QSqlQuery>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QLatin1String>
#include <QSqlTableModel>
#include <QDate>
#include <QtSql>
#include <QStringList>
#include <QDebug>


using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::vector;
using std::cin; using std::istringstream;
using std::stringstream;
using namespace std;

namespace Ui {
class GPS;
}


class GPS : public QMainWindow
{
    Q_OBJECT

    public :
        explicit GPS(QWidget *parent = 0);
        virtual ~GPS();
        void RecupererTrame();
        void AnalyserTrame(QString);
        void ConstruireBase();
        void addDataGPS(QString heure, QString latitude, QString longitude, QString altitude);
        void Afficher();




    private :
        Ui::GPS *ui;
        QString trame;
        QString delimiter;
        QString type_trame;
        QString heurebrut;
        QString heure;
        QString latitude;
        QString longitude;
        QString altitude;
        QString portion;
        QSqlQuery *ptr;
        QStringList array;
};

#endif // GPS_H
