#include "recup_gyroscope.h"
#include "ui_recup_gyroscope.h"

Recup_Gyroscope::Recup_Gyroscope(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Recup_Gyroscope)
{
    ui->setupUi(this);
    detectIMU();
    enableIMU();
    rate_gyr_y =0.0;   // [deg/s]
    rate_gyr_x = 0.0;   // [deg/s]
    rate_gyr_z = 0.0;   // [deg/s]
    gyroXangle = 0.0;
    gyroYangle = 0.0;
    gyroZangle = 0.0;
    AccYangle = 0.0;
    AccXangle = 0.0;
}

Recup_Gyroscope::~Recup_Gyroscope()
{
    delete ui;
}
Recup_Gyroscope::accgyro()
{
    detectIMU();
    enableIMU();
    rate_gyr_y =0.0;   // [deg/s]
    rate_gyr_x = 0.0;   // [deg/s]
    rate_gyr_z = 0.0;   // [deg/s]
    gyroXangle = 0.0;
    gyroYangle = 0.0;
    gyroZangle = 0.0;
    AccYangle = 0.0;
    AccXangle = 0.0;
}


QString Recup_Gyroscope::Getangles()
{
    return angles;
}

int Recup_Gyroscope::calculer_angles()
{

    int  gyrRaw[3];
    int  accRaw[3];
    int  magRaw[3];

    angles =QString();

    readGYR(gyrRaw);
    readACC(accRaw);
    readMAG(magRaw);

    rate_gyr_x = (float) gyrRaw[0] * G_GAIN;
    rate_gyr_y = (float) gyrRaw[1]  * G_GAIN;
    rate_gyr_z = (float) gyrRaw[2]  * G_GAIN;

    AccXangle = (float) (atan2(accRaw[1],accRaw[2])+M_PI)*RAD_TO_DEG;
    AccYangle = (float) (atan2(accRaw[2],accRaw[0])+M_PI)*RAD_TO_DEG;


    magRawX = (float)magRaw[0];
    magRawY = (float)magRaw[1];
    magRawZ = (float)magRaw[2];

    //Calculate the angles from the gyro
    gyroXangle+=rate_gyr_x*DT;
    gyroYangle+=rate_gyr_y*DT;
    gyroZangle+=rate_gyr_z*DT;

    AccXangle -= (float)180.0;
        if (AccYangle > 90)
            AccYangle -= (float)270;
        else
            AccYangle += (float)90;

        //Compute heading
        float heading = 180 * atan2(magRaw[1],magRaw[0])/M_PI;

        //Convert heading to 0 - 360
        if(heading < 0)
            heading += 360;
        //qDebug ()<< heading;

    QTextStream out(&angles);
    out<<gyroXangle<<";"<<gyroYangle<<";"<<gyroZangle<<";"<<AccXangle<<";"<<AccYangle<<";"<<magRawX<<";"<<magRawY<<";"<<magRawZ;

}

void Recup_Gyroscope::readGYR(int *g)
{
    uint8_t block[6];
        selectDevice(file,LSM9DS1_GYR_ADDRESS);
    readBlock(0x80 |  LSM9DS1_OUT_X_L_G, sizeof(block), block);

    // Combine readings for each axis.
    *g = (int16_t)(block[0] | block[1] << 8);
    *(g+1) = (int16_t)(block[2] | block[3] << 8);
    *(g+2) = (int16_t)(block[4] | block[5] << 8);
}
void Recup_Gyroscope::detectIMU()
{

    //__u16 block[I2C_SMBUS_BLOCK_MAX];

//	int res, bus,  size;


    char filename[20];
    sprintf(filename, "/dev/i2c-%d", 1);
    file = open(filename, O_RDWR);
    if (file<0) {
        exit(1);
    }

        //Detect if BerryIMUv2 (Which uses a LSM9DS1) is connected
    selectDevice(file,LSM9DS1_MAG_ADDRESS);
    //int LSM9DS1_WHO_M_response = i2c_smbus_read_byte_data(file, LSM9DS1_WHO_AM_I_M);

    selectDevice(file,LSM9DS1_GYR_ADDRESS);
    //int LSM9DS1_WHO_XG_response = i2c_smbus_read_byte_data(file, LSM9DS1_WHO_AM_I_XG);



}




void Recup_Gyroscope::enableIMU()
{


        // Enable the gyroscope
        writeGyrReg(LSM9DS1_CTRL_REG4,0b00111000);      // z, y, x axis enabled for gyro
        writeGyrReg(LSM9DS1_CTRL_REG1_G,0b10111000);    // Gyro ODR = 476Hz, 2000 dps
        writeGyrReg(LSM9DS1_ORIENT_CFG_G,0b10111000);   // Swap orientation



