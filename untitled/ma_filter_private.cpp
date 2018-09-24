#include "ma_filter_private.h"
#include <QFile>
#include <QDebug>

MA_Filter_private::MA_Filter_private(QObject *parent):QObject(parent){
    this->read_koeff("koef.dat");
}

MA_Filter_private::~MA_Filter_private(){
    delete databuffer;
}
MA_Filter_private::MA_Filter_private(QString keffFilePath, QObject *parent) :QObject(parent){
    this->read_koeff(keffFilePath);
}
void MA_Filter_private::read_koeff(QString path){
    QFile inFile(path);
    if(!inFile.exists()){
        this->koeff.push_back(1);// if not file is present make a bypass
    }
    inFile.open(QIODevice::ReadOnly);// open the File
    QString line;
    while( !(line = QString::fromLatin1(inFile.readLine())).isEmpty()){
        bool ok;
        qreal res;
        res = line.toDouble(&ok);
        if(ok){
            this->koeff.push_back(res);
        }
    }
    qDebug() << "Read Koeff file: " << inFile.fileName();
    inFile.close();
    // set the Ring storate to the desired size and delete it if already there
    if(this->databuffer != nullptr)
        delete this->databuffer;
    qDebug() << "Setup Filter with " << QString::number(this->koeff.size()) << "Koeff";
    this->databuffer = new RingStorage<qreal>(this->koeff.size());
    // init the Buffer with 0
    for(int i = 0; i < this->koeff.size();i++){
        this->databuffer->put_data(0.0);
    }
}


qreal MA_Filter_private::processNewValue(qreal data){
    qreal resultValue = data;
    for(int i = 0;i < this->koeff.size(); i++)
    {        
        resultValue += this->databuffer->at(i) * this->koeff[i];

    }
    // push the data back into the buffer
    this->databuffer->put_data(data);
    return resultValue / (this->koeff.size() + 1);
}
void MA_Filter_private::setKoeffPath(QString path){
    // Mapping to Private Function
    this->read_koeff(path);
}
