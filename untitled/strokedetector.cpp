#include "strokedetector.h"
#include "gyrosensor.h"
#include "ma_filter.h"

#define msecs_in_minute 60000

StrokeDetector::StrokeDetector(QObject *parent) : QObject(parent)
{
    // generate filter an Gyro sensor and connect them together
    MA_Filter* fil = new MA_Filter(this);
    GyroSensor* sen = new GyroSensor(this);
    timer.start();
    connect(sen,SIGNAL(newSensorValue(GyroData)),fil,SLOT(updateInput(GyroData)));
    connect(fil,SIGNAL(updatedOutput(qreal)),this,SLOT(processnewvalue(qreal)));
}

void StrokeDetector::processnewvalue(qreal newValue){
    if(newValue < 4.0){
        // get time between to time stamps
        qint32 ellapsmsec = timer.elapsed();
        timer.restart();
        ellapsmsec = msecs_in_minute / ellapsmsec; // dirived from ---> msecs_in_minute = spm * ellapsmsec
        quint8 res = ellapsmsec & 0xff;
        emit StrokeUpdate(res);
    }
}
