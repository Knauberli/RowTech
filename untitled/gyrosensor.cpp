#include "gyrosensor.h"
#include "gyrosensorprivate.h"
#include <QDebug>
#include <QTimer>


/*
 * following includes are used for handling IO with the standard i2c bus driver
 *
*/



GyroSensor::GyroSensor(QObject *parent) : QObject(parent)
{
    this->pri = new GyroSensorPrivate();
    connect(&this->timer,SIGNAL(timeout()),this,SLOT(readsensorData()));
    this->timer.setInterval(1000);
    this->timer.setSingleShot(false);
    this->timer.setTimerType(Qt::PreciseTimer); // divvers 1 ms accurate
    this->timer.start();

}

GyroSensor::~GyroSensor(){
    delete this->pri;
}

void GyroSensor::readsensorData(){
    GyroData data = this->pri->readData();
    emit this->newSensorValue(data);
}

void GyroSensor::setPollingInterval(quint32 msec){
    this->timer.setInterval(msec);
}
