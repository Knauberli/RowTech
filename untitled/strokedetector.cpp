#include "strokedetector.h"
#include "gyrosensor.h"
#include "ma_filter.h"
#include "ringstorage.h"
#define msecs_in_minute 60000

StrokeDetector::StrokeDetector(QObject *parent) : QObject(parent)
{
    // generate filter an Gyro sensor and connect them together
    MA_Filter* fil = new MA_Filter(this);
    GyroSensor* sen = new GyroSensor(this);
    timer.start();
    connect(sen,SIGNAL(newSensorValue(GyroData)),fil,SLOT(updateInput(GyroData)));
    connect(fil,SIGNAL(updatedOutput(qreal)),this,SLOT(processnewvalue(qreal)));
    this->downSampleVlaues = new RingStorage<qreal>(2);

}

StrokeDetector::~StrokeDetector(){
    delete this->downSampleVlaues;
}
// get the local minimum of the signal as reference point and calulate

void StrokeDetector::rundetection(){
    // if the last element is lower than the element before we passed a minimum
    if( this->downSampleVlaues->at(this->downSampleVlaues->size()-1) > this->downSampleVlaues->at(this->downSampleVlaues->size() - 2) ){
        // get time between to time stamps
        qint32 ellapsmsec = timer.elapsed();
        timer.restart();
        ellapsmsec = msecs_in_minute / ellapsmsec; // dirived from ---> msecs_in_minute = spm * ellapsmsec
        quint8 res = ellapsmsec & 0xff;
        emit StrokeUpdate(res);
    }
}
void StrokeDetector::processnewvalue(qreal newValue){
    static qint8 dscounter = 0; // init in static function --> creepy shit
    if( (dscounter++ % ds_ration) == 0){
        this->downSampleVlaues->put_data(newValue);
        rundetection();
    }

}
