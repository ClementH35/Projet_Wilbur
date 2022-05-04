#include "gps.h"
#include "ui_gps.h"

GPS::GPS(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GPS)
    {
    ui ->setupUi(this);
    cout << "Constructeur" << endl;
    delimiter=",";
    ConstruireBase();
    RecupererTrame();

    }

GPS::~GPS()
{
    delete ui;
}

void GPS::RecupererTrame()
{

    QFile fichier("trame.txt");
    //QThread::sleep(5);
    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        while(!flux.atEnd())
        {
            trame = flux.readLine();
            //qDebug() << trame;
            AnalyserTrame(trame);
        }
        fichier.close();
    }
}

void GPS::AnalyserTrame(QString trame)
{
    QStringList contenu = trame.split(delimiter);
    type_trame = contenu.at(0);
    if (type_trame == "$GPGGA")
    {

        heurebrut = contenu.at(1);
        heure += heurebrut[0];
        heure += heurebrut[1];
        heure += 'h';
        heure += heurebrut[2];
        heure += heurebrut[3];
        heure += 'm';
        heure += heurebrut[4];
        heure += heurebrut[5];
        heure += 's';

        latitude = contenu.at(2) + ','+contenu.at(3);
        longitude = contenu.at(4) + ','+contenu.at(5);
        altitude = contenu.at(9) + ','+contenu.at(10);
        qDebug() << heure << latitude << longitude << altitude;
    }
    if (type_trame == "$GPRMC")
    {
        heurebrut = contenu.at(1);
        heure += heurebrut[1];
        heure += 'h';
        heure += heurebrut[2];
        heure += heurebrut[3];
        heure += 'm';
        heure += heurebrut[4];
        heure += heurebrut[5];
        heure += 's';

        latitude = contenu.at(3)+','+contenu.at(4);
        longitude = contenu.at(5)+','+contenu.at(6);
        altitude = "NoData";
        qDebug() << heure << latitude << longitude;
    }
    addDataGPS(heure, latitude, longitude, altitude);
    heure = "0";
    latitude = "0";
    longitude = "0";
    altitude = "0";
}

void GPS::ConstruireBase()
{
    cout << "Création base de données" <<endl;
    QSqlDatabase  Base = QSqlDatabase::addDatabase("QSQLITE");
    Base.setDatabaseName("base.sqlite3");
    Base.setUserName("wilbur");
    Base.setPassword("wilbur");
    Base.open();
    cout << "BDD Crée !" << endl;

    ptr = new QSqlQuery(Base);

    ptr->exec(QLatin1String("create table donnees_gps(id integer primary key,heure real,latitude real, longitude real, altitude real)"));
    ptr->exec(QLatin1String("create table photos(id integer primary key,nom real,format real)"));
    ptr->exec();
}

void GPS::addDataGPS(QString heure, QString latitude, QString longitude, QString altitude)
{  qDebug() << "Ajout des données";
    /*QSqlDatabase Base = QSqlDatabase::database();
    ptr = new QSqlQuery(Base);*/
    ptr->prepare("INSERT INTO donnees_gps (heure, latitude, longitude, altitude) "
                      "VALUES (?, ?, ?, ?)");
       ptr->addBindValue(heure);
       ptr->addBindValue(latitude);
       ptr->addBindValue(longitude);
       ptr->addBindValue(altitude);
       ptr->exec();
}

/*void GPS::Afficher()
{
     QSqlRelationalTableModel *model = new QSqlRelationalTableModel(this);
     model->setEditStrategy(QSqlTableModel::OnManualSubmit);
     model->setTable("donnees_gps");
     int heureId = model->fieldIndex("heure");
     int latitudeId = model->fieldIndex("latitude");
     int longitudeId = model->fieldIndex("longitude");
     int altitudeId = model->fieldIndex("altitude");
     model->setRelation(auteurId, QSqlRelation("Auteurs", "id", "nom"));
     model->setRelation(typeId, QSqlRelation("Genres", "id", "nom"));
     model->select();


      --- model->setTable("Livres");
      model->setEditStrategy(QSqlTableModel::OnManualSubmit);
      model->select(); ---
      model->setHeaderData(1, Qt::Horizontal, tr("titre"));
      model->setHeaderData(2, Qt::Horizontal, tr("auteur"));
      model->setHeaderData(3, Qt::Horizontal, tr("genre"));
      model->setHeaderData(4, Qt::Horizontal, tr("annee"));
      model->setHeaderData(5, Qt::Horizontal, tr("evaluation"));
      ui->tableView->setModel(model);
      //ui->lineEdit->setText();

}*/

/*void GPS::Supprimer_BDD()
{
    DROP DATABASE base
}*/

