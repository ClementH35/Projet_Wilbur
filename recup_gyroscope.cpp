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


}


int file;


void readBlock(uint8_t command, uint8_t size, uint8_t *data)
        {
            int result = i2c_smbus_read_i2c_block_data(file, command, size, data);
            if (result != size){
                exit(1);
            }
        }


void selectDevice(int file, int addr)
        {
            if (ioctl(file, I2C_SLAVE, addr) < 0) {
                }
        }


        void readACC(int  *a)
        {
            uint8_t block[6];
            selectDevice(file,LSM9DS1_ACC_ADDRESS);
            readBlock(0x80 |  LSM9DS1_OUT_X_L_XL, sizeof(block), block);

            // Combine readings for each axis.
            *a = (int16_t)(block[0] | block[1] << 8);
            *(a+1) = (int16_t)(block[2] | block[3] << 8);
            *(a+2) = (int16_t)(block[4] | block[5] << 8);

        }


        void readMAG(int  *m)
        {
            uint8_t block[6];
                selectDevice(file,LSM9DS1_MAG_ADDRESS);
            readBlock(0x80 |  LSM9DS1_OUT_X_L_M, sizeof(block), block);


            // Combine readings for each axis.
            *m = (int16_t)(block[0] | block[1] << 8);
            *(m+1) = (int16_t)(block[2] | block[3] << 8);
            *(m+2) = (int16_t)(block[4] | block[5] << 8);

        }

        void readGYR(int *g)
        {
            uint8_t block[6];
                selectDevice(file,LSM9DS1_GYR_ADDRESS);
            readBlock(0x80 |  LSM9DS1_OUT_X_L_G, sizeof(block), block);

            // Combine readings for each axis.
            *g = (int16_t)(block[0] | block[1] << 8);
            *(g+1) = (int16_t)(block[2] | block[3] << 8);
            *(g+2) = (int16_t)(block[4] | block[5] << 8);
        }


        void writeAccReg(uint8_t reg, uint8_t value)
        {
            selectDevice(file,LSM9DS1_ACC_ADDRESS);

            int result = i2c_smbus_write_byte_data(file, reg, value);
            if (result == -1){

                exit(1);
            }
        }

        void readTMP_PRESS(int *g)
        {
            uint8_t block[6];
            selectDevice(file,BMP280_ADDR);
            readBlock(0x80 |  BMP280_PRESSION_MSB , sizeof(block), block);
            g[0] = ((block[0] * 65536) + (block[1] * 256) + (block[2] & 0xF0)) / 16;
            g[1] = ((block[3] * 65536) + (block[4] * 256) + (block[5] & 0xF0)) / 16;
        }

        void readCalBMP280(int16_t *Reg)
        {
          uint8_t block[24];
          selectDevice(file,BMP280_ADDR);
          readBlock(0x80 |  BMP_280_REG_COMP_T1, sizeof(block), block);
          // Calibrage Température & Pression
          Reg[0] = block[1] * 256 + block[0];

          for(int i=1,j=2;i<3;i++,j++)
                { if ((block[i+j] * 256 + block[j+i-1]) > 32767)  Reg[i] = (block[i+j] * 256 + block[j+i-1]) - 65536;
                    else  Reg[i] = block[i+j] * 256 + block[j+i-1];
                }

          Reg[3] = block[7] * 256 + block[6];

           for(int i=4,j=5;i<12;i++,j++)
           {
               if ((block[i+j] * 256 + block[j+i-1]) > 32767)  Reg[i] = (block[i+j] * 256 + block[j+i-1]) - 65536;
               else  Reg[i] = block[i+j] * 256 + block[j+i-1];
           }
        }

        void writeTMP()
        {
            //uint8_t block[2];
            selectDevice(file,BMP280_ADDR);


            int result1 = i2c_smbus_write_byte_data(file,BMP280_REG_CONTROL_MEAS,0x27);
            int result2 = i2c_smbus_write_byte_data(file,BMP280_REG_CONFIG,0xA0);
            if (result1 == -1 || result2 == -1){
                printf ("Failed to write byte to I2C Acc.");
                exit(1);
            }
        }

        void writeMagReg(uint8_t reg, uint8_t value)
        {
                selectDevice(file,LSM9DS1_MAG_ADDRESS);

            int result = i2c_smbus_write_byte_data(file, reg, value);
            if (result == -1){
            exit(1);
            }
        }


        void writeGyrReg(uint8_t reg, uint8_t value)
        {
                selectDevice(file,LSM9DS1_GYR_ADDRESS);

            int result = i2c_smbus_write_byte_data(file, reg, value);
            if (result == -1){
                exit(1);
            }
        }
