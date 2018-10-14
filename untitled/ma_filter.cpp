#include "ma_filter.h"
#include "ma_filter_private.h"
#include <QFile>
#include <math.h>
// we have to write the private data into a header so the moc do the procssing about it
#define EST_FILEPATH "LongTermEst.dat"
MA_Filter::MA_Filter(QObject *parent) : QObject(parent)
{
    this->priv = new MA_Filter_private[6];
    this->g_comprassion = new MA_Filter_private[6];
    // By decaling it in a plain vector it isn't possible to set the parent pointer to allow automatic
    // destruction via qt Framework
    for(int i = 0; i <6;i++){
        this->priv[i].setParent(this);
    }
    QFile fp(EST_FILEPATH);
    if(!fp.exists()){
        fp.open(QIODevice::ReadWrite);
        for(int i = 0 ; i < 500; i++){
            fp.write("1\n");
        }
        fp.close();
    }
    for(int i = 0; i < 6; i++){
        this->g_comprassion[i].setParent(this);
        this->g_comprassion[i].setKoeffPath(EST_FILEPATH);
    }


}

// process the first Step of singnal process
void MA_Filter::filter_layer_1(GyroData &data){
    data.AccelX = this->priv[0].processNewValue(data.AccelX);
    data.AccelY = this->priv[1].processNewValue(data.AccelY);
    data.AccelZ = this->priv[2].processNewValue(data.AccelZ);
    data.GyroX = this->priv[3].processNewValue(data.GyroX);
    data.GyroY = this->priv[4].processNewValue(data.GyroY);
    data.GyroZ = this->priv[5].processNewValue(data.GyroZ);
    //print_GyroData(data);
}

void MA_Filter::filter_g(GyroData &data){
    this->g_estimation.AccelX = this->g_comprassion[0].processNewValue(data.AccelX);
    this->g_estimation.AccelY = this->g_comprassion[1].processNewValue(data.AccelY);
    this->g_estimation.AccelZ = this->g_comprassion[2].processNewValue(data.AccelZ);
    this->g_estimation.GyroX = this->g_comprassion[3].processNewValue(data.GyroX);
    this->g_estimation.GyroY = this->g_comprassion[4].processNewValue(data.GyroY);
    this->g_estimation.GyroZ = this->g_comprassion[5].processNewValue(data.GyroZ);

}

void MA_Filter::updateInput(GyroData& data){
    //this->filter_layer_1(data);
    //this->filter_g(data);
    //print_GyroData(data);
    // genterate the vector to the driven direction without the gravity
    // ret.AccelY = data.AccelY - this->g_estimation.AccelY;
    // ret.AccelZ = data.AccelZ - this->g_estimation.AccelZ;
    // calculate the value of the vetor
    //qDebug() << sqrt(pow(ret.AccelY,2) + pow(ret.AccelZ,2));
    qreal erg = sqrt(pow(data.AccelY,2) + pow(data.AccelZ,2));
    // if the y vertor is negative we got an acceration into the wrong direction

    if(data.AccelY < 0 )
        erg *= -1;
    emit this->updatedOutput(erg);

}
