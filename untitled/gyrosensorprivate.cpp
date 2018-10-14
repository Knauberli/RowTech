#include "gyrosensorprivate.h"
#include "gyrosensor.h"
#include "defines.h"
// QT Includes

#include <QDebug>
#include <QFile>

// C Util Functions
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "gravityfilter.h"

#define CUTOFF_HIGHPASS 15
GyroSensorPrivate::GyroSensorPrivate(QObject *parent) : QObject(parent)
{
    this->SetupI2C();
    this->gv = new GravityFilter(this);
    this->gv->setCuttoff(CUTOFF_HIGHPASS);
    this->gv->setSampleFrequency(200);
}
GyroSensorPrivate::GyroSensorPrivate(QString devFilepath, QObject *parent):QObject(parent){
    this->filePath = devFilepath;
    this->SetupI2C();
}
GyroSensorPrivate::~GyroSensorPrivate(){
}
bool GyroSensorPrivate::isReady(){
    if(this->state_ == READY){
        return true;
    }
    return false;
}

void GyroSensorPrivate::SetupI2C(){
    if(this->filePath.isEmpty() || this->filePath.isNull()){
        this->filePath = I2C_GYRO_DEFAULT_BUS; // if filepath is not set Set it to defautl path
    }
    QFile testExist(this->filePath);
    if(!testExist.exists()){
        qDebug() << this->filePath << " Bus doesn't exist";
        this->state_ = ERROR;
        return;
    }
    this->filedescriptor = open(this->filePath.toLatin1().data(),O_RDWR); // Try to open the i2c file in /dev filder
    if(this->filedescriptor < 0 ){
        // if there is an Error open will return with an negative value
        qDebug() << "Error while opening the bus";
        qDebug() << strerror(errno);// print system Error message
        this->state_ = ERROR;
        return;
    }
    if(ioctl(this->filedescriptor,I2C_SLAVE,I2C_GYRO_SLAVE_ADDRES) < 0){
        qDebug() << "Error while Setting Device as Slave";
        qDebug() << strerror(errno);
        this->state_ = ERROR;
        return;
    }
    qInfo()<< "Open I2C bus";

    char buffer[2] ;
    // set the sensivity to +/- 4 g
    buffer[0] = REG_ACCEL_CONFIG ;
    buffer[1] = 0 | (1 << 3);
    write(this->filedescriptor,buffer,2);
/*
    // enable fifo for reading
    buffer[0] = REG_FIFO_EN;
    buffer[1] = 0 | (1 << 3);
    write(this->filedescriptor,buffer,2);

    buffer[0] = REG_USER_CTRL;
    buffer[1] = 0 | (1 << 6);
    write(this->filedescriptor,buffer,2);
*/
    uchar gyroConfig = this->getRegisterValue(REG_GYRO_CONFIG)&0x17;
    switch (gyroConfig) {
    case 0x00:
        this->GyroFactor = GYRO_SHORT_TO_DOUBLE_FACTOR_0;
        break;
    case 0x07:
        this->GyroFactor = GYRO_SHORT_TO_DOUBLE_FACTOR_1;
        break;
    case 0x10:
        this->GyroFactor = GYRO_SHORT_TO_DOUBLE_FACTOR_2;
        break;
    case 0x17:
        this->GyroFactor = GYRO_SHORT_TO_DOUBLE_FACTOR_3;
        break;
    default:
        this->GyroFactor = GYRO_SHORT_TO_DOUBLE_FACTOR_0;
        break;
    }

    uchar accelConfig = this->getRegisterValue(REG_ACCEL_CONFIG);
    qDebug() << "AccelConfig " << QString::number(accelConfig,16);
    accelConfig &= 0x17;
    switch (accelConfig) {
    case 0x00:
        this->AccelFactor = ACCEL_SHORT_TO_DOUBLE_FACTOR_0;
        break;
    case 0x07:
        this->AccelFactor = ACCEL_SHORT_TO_DOUBLE_FACTOR_1;
        break;
    case 0x10:
        this->AccelFactor = ACCEL_SHORT_TO_DOUBLE_FACTOR_2;
        break;
    case 0x17:
        this->AccelFactor = ACCEL_SHORT_TO_DOUBLE_FACTOR_3;
        break;
    default:this->AccelFactor = ACCEL_SHORT_TO_DOUBLE_FACTOR_0;
        break;
    }
    qDebug() << "Accel Factor is set to " << this->AccelFactor << "\n GyroFactor is set to " << this->GyroFactor;
    // Sensor power on
    buffer[0] = REG_PWR_MGMT_1,
    buffer[1]=0;
    write(this->filedescriptor,buffer,2);
    this->state_ = READY;

}

void GyroSensorPrivate::reopenBus(){
    if(this->state_ == CLOSED){
        this->SetupI2C();
    }
}
void GyroSensorPrivate::closeBus(){
    if(this->state_ == READY){
        close(this->filedescriptor);
        this->state_ = CLOSED;
    }
}
/*
 * readData returns an Struct containing all values form the Sensor as struct
 * an Struct with 0 will retrund on error
*/
_GyroData GyroSensorPrivate::readData(){
    short accelx=0,accely=0,accelz=0;
    short gyrox=0,gyroy=0,gyroz=0;

    struct _GyroData retdata = {.AccelX=0,.AccelY=0,.AccelZ=0,.GyroX=0,.GyroY=0,.GyroZ=0};
    if(this->state_ == ERROR){
        qInfo() << "Bus is in Error State";
        return retdata;
    }

    /* GET all Accelormeter Values form the REgisters */
    accelx = this->getRegisterValue(REG_ACCEL_XOUT_L);
    accelx |= this->getRegisterValue(REG_ACCEL_XOUT_H)<< 8;
    accely = this->getRegisterValue(REG_ACCEL_YOUT_L);
    accely |= this->getRegisterValue(REG_ACCEL_YOUT_H) << 8;
    accelz = this->getRegisterValue(REG_ACCEL_ZOUT_L);
    accelz |= this->getRegisterValue(REG_ACCEL_ZOUT_H) << 8;
    /*GET all Gyro Values form the Registers */
    gyrox = this->getRegisterValue(REG_GYRO_XOUT_L);
    gyrox |= this->getRegisterValue(REG_GYRO_XOUT_H) << 8;
    gyroy = this->getRegisterValue(REG_GYRO_YOUT_L);
    gyroy |=this->getRegisterValue(REG_GYRO_YOUT_H) << 8;
    gyroz = this->getRegisterValue(REG_GYRO_ZOUT_L);
    gyroz |= this->getRegisterValue(REG_GYRO_ZOUT_H) << 8;


    retdata.AccelX = accelx ;
    retdata.AccelY = accely ;
    retdata.AccelZ = accelz ;

    retdata.GyroX = gyrox ;
    retdata.GyroY = gyroy ;
    retdata.GyroZ = gyroz ;
    this->calculateRelativeValues(&retdata);
    this->gv->filterGravity(retdata);
    print_GyroData(retdata);

    return retdata;
}

uchar GyroSensorPrivate::getRegisterValue(uchar RegisterNumber){
    uchar byte = RegisterNumber;
    write(this->filedescriptor,&byte,1);
    read(this->filedescriptor,&byte,1);
    return byte;
}
void GyroSensorPrivate::calculateRelativeValues(struct _GyroData *data){
#ifndef GRAVITY_CONSTANT
    data->AccelX = data->AccelX * this->AccelFactor; // *.0 to force conversion into double value the magic onstant came from the DOC
    data->AccelY = data->AccelY * this->AccelFactor;
    data->AccelZ = data->AccelZ * this->AccelFactor;
#else
    data->AccelX = data->AccelX / this->AccelFactor * GRAVITY_CONSTANT; // *.0 to force conversion into double value the magic onstant came from the DOC
    data->AccelY = data->AccelY / this->AccelFactor * GRAVITY_CONSTANT;
    data->AccelZ = data->AccelZ / this->AccelFactor * GRAVITY_CONSTANT;
#endif
    data->GyroX = (data->GyroX / this->GyroFactor);
    data->GyroY = (data->GyroY / this->GyroFactor);
    data->GyroZ = (data->GyroZ / this->GyroFactor);

}
uchar* GyroSensorPrivate::readBurstDataFifo(){
    uchar* data= nullptr;
    quint16 fifocount = this->getRegisterValue(REG_FIFO_COUNT_H) << 8;
    fifocount |= this->getRegisterValue(REG_FIFO_COUNT_L);
    data = new uchar[fifocount];
    read(this->filedescriptor,data,fifocount);
    return data;
}
