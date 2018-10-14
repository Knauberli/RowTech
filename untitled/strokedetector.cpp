#include "strokedetector.h"
#include "gyrosensor.h"
#include "ma_filter.h"
#include "ringstorage.h"
#include "math.h"
#define msecs_in_minute 60000
// just some random factor to avoid noise into the singal
#define strength_per_stroke 0.8
StrokeDetector::StrokeDetector(QObject *parent) : QObject(parent)
{
    // generate filter an Gyro sensor and connect them together
    MA_Filter* fil = new MA_Filter(this);
    GyroSensor* sen = new GyroSensor(this);
    sen->setPollingInterval(5); // this equals 200Hz
    timer.start();
    connect(sen,SIGNAL(newSensorValue(GyroData&)),fil,SLOT(updateInput(GyroData&)));
    connect(fil,SIGNAL(updatedOutput(qreal)),this,SLOT(processnewvalue(qreal)));
    this->downSampleVlaues = new RingStorage<qreal>(2);

}

StrokeDetector::~StrokeDetector(){
    delete this->downSampleVlaues;
}
// get the local minimum of the signal as reference point and calulate

void StrokeDetector::rundetection(){
    // if the last element is lower than the element before we passed a minimum
    // qDebug() << this->downSampleVlaues->at(this->downSampleVlaues->size()-1) << this->downSampleVlaues->at(this->downSampleVlaues->size() - 2);
    // we poi if the dirrection of the verctor changes his sign
    // https://aticleworld.com/5-ways-to-check-if-two-integers-have-opposite-signs/
    // check if the strength is enought for a direction change

    if(fabs(this->downSampleVlaues->at(this->downSampleVlaues->size()-1)) < strength_per_stroke){
        return;
    }
    if(! ((this->downSampleVlaues->at(this->downSampleVlaues->size()-1) >=0 ) ^ (this->downSampleVlaues->at(this->downSampleVlaues->size() - 2) < 0)) ){
        // get time between to time stamps
        qint32 ellapsmsec = timer.elapsed();
        timer.restart();
        qDebug() << "All good so far";
        ellapsmsec = msecs_in_minute / ellapsmsec; // dirived from ---> msecs_in_minute = spm * ellapsmsec
        quint8 res = ellapsmsec & 0xff;
        qDebug() << "Guessed Stroke " << res;
        emit StrokeUpdate(res);
    }
}
void StrokeDetector::processnewvalue(qreal newValue){
    static qint8 dscounter = 0; // init in static function --> creepy shit
    if((dscounter++ % ds_ration) == 0){
        this->downSampleVlaues->put_data(newValue);
        rundetection();
    }

}
