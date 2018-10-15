#include "gps.h"
#include <QFile>
#include <QStringList>
#include <QRegExp>
#include <QTimer>
#include <QDebug>

GPS::GPS(QObject *parent) : QObject(parent)
{
    this->m_devicepath = "/dev/GPS"; // set to path of the device driver
    this->setupTimeing();
}
GPS::GPS(QString path, QObject *parent) : QObject(parent){
    this->m_devicepath = path;
    this->setupTimeing();

}

inline void GPS::setupTimeing(){
    this->device = new QFile(this->m_devicepath,this);
    if(this->device == nullptr)
        return;
    if(!this->device->exists()){
        qDebug() << "Device File not Present programm may fail";
    }
    this->device->open(QIODevice::ReadOnly);
    timer.setSingleShot(false);
    timer.setInterval(100);
    connect(&timer,SIGNAL(timeout()),this,SLOT(parseData()));
    timer.start();
    qDebug() << "GPS Module ready to Read";
    return;
 }
GPS::~GPS(){
    this->device->close();
    delete this->device;
}
qreal GPS::getLatitude(){
    qreal retval;
    this->lock_values.lock();
    retval = this->m_latidude;
    this->lock_values.unlock();
    return retval;
}
qreal GPS::getLongitude(){
    qreal retval;
    this->lock_values.lock();
    retval = this->m_longintude;
    this->lock_values.unlock();
    return retval;
}
Orinetation GPS::getOrientation(){
    enum Orinetation_ retval;
    this->lock_values.lock();
    retval = this->m_orinet;
    this->lock_values.unlock();
    return retval;
}

void GPS::parseData(){

    QByteArray readbuffer;
    quint8 ch;    
    //qDebug() << "start Reading";
    while(this->device->getChar((char* )&ch)){
        readbuffer.append(ch);
        if(ch == '\n'){
            //qDebug() << "Got Line";
            break;
        }
    }
    qDebug()<< QString(readbuffer);
    readbuffer = QString(readbuffer).trimmed().toLatin1();
    QStringList list = QString(readbuffer).split(",");
    if(list.size() == 0){
        qDebug() <<"Error with the InputString";
    }
    // last element contains \0
    for(int i=0; i<readbuffer.size() -1 ;i++){
        // check if one value is not in the ascii table
        if((readbuffer.at(i) < ' ' || readbuffer.at(i) > '~') && readbuffer.at(i) != '\0'){
            return;
        }
    }
    if(!list.at(0).contains("GPGGA")){
        qDebug()<<"Not A coordinate String";
        return;
    }
    qDebug()<<"Start Parsing";
    qreal n_longitude = list.at(2).toDouble();
    qreal n_latidude = list.at(4).toDouble();
    quint16 dir = list.at(3).toShort();
    dir += list.at(5).toShort();
    Orinetation nori;
    switch (dir) {
    case 'N'+'E':
        nori = Orinetation::NE;
        break;
    case 'N' + 'W':
        nori = Orinetation::NW;
        break;
    case 'S' + 'W':
        nori = Orinetation::SW;
        break;
    case 'S' + 'E':
        nori = Orinetation::SE;
        break;
    default:
        nori = Orinetation::NE;
        break;
    }
    qDebug() << "Long: " << n_longitude << "Lat: " << n_latidude;
    this->lock_values.lock();
    this->m_latidude = n_latidude;
    this->m_longintude = n_longitude;
    this->m_orinet = nori;
    this->lock_values.unlock();
    return;
    timer.start(50);
}
