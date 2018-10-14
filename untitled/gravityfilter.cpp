#include "gravityfilter.h"
#include <math.h>
#include <QDebug>

#define PI2 2*M_PI
GravityFilter::GravityFilter(QObject *parent) : QObject(parent)
{

}
void GravityFilter::calculateAlpth(){
    this->alpha = this->cuttoff / (this->cuttoff + this->deltatime);
    qDebug() << "Set new Alpah:" << this->alpha;
}
void GravityFilter::setCuttoff(qreal Cuttoff){
    this->cuttoff = 1/(Cuttoff * PI2);
    this->calculateAlpth();
}
void GravityFilter::setSampleFrequency(qreal Deltatime){
    this->deltatime = 1/Deltatime;
    this->calculateAlpth();
}
void GravityFilter::filterGravity(GyroData &data){
    // calculate the new Gravity Values
    this->gravity.AccelX = this->alpha * this->gravity.AccelX
            + (1 - this->alpha) * data.AccelX;
    this->gravity.AccelY = this->alpha * this->gravity.AccelY
            + (1 - this->alpha) * data.AccelY;
    this->gravity.AccelZ = this->alpha * this->gravity.AccelZ
            + (1 - this->alpha) * data.AccelZ;
    // substract the Gravity form the sensor values
    data.AccelX -= this->gravity.AccelX;
    data.AccelY -= this->gravity.AccelY;
    data.AccelZ -= this->gravity.AccelZ;
}
