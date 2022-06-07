#ifndef RECUP_GYROSCOPE_H
#define RECUP_GYROSCOPE_H

#include <QMainWindow>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include </usr/include/unistd.h>
#include </usr/include/math.h>
#include </usr/include/stdlib.h>
#include </usr/include/fcntl.h>
#include <QString>
#include <QTextStream>
#include </usr/include/time.h>

namespace Ui {
class Recup_Gyroscope;
}

class Recup_Gyroscope : public QMainWindow
{
    Q_OBJECT

public:
    explicit Recup_Gyroscope(QWidget *parent = 0);
    ~Recup_Gyroscope();
    void detectIMU();
    void enableIMU();
    int mymillis();
    int calculer_angles();
    QString Getangles();
    void readGYR(int *g);

private:
    Ui::Recup_Gyroscope *ui;
    float rate_gyr_y;   // [deg/s]
    float rate_gyr_x;   // [deg/s]
    float rate_gyr_z;   // [deg/s]
    float gyroXangle;
    float gyroYangle;
    float gyroZangle;
    float AccYangle;
    float AccXangle;
    float magRawX;
    float magRawY;
    float magRawZ;
    QString angles;
};

#endif // RECUP_GYROSCOPE_H
