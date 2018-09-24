#ifndef GYROSENSOR_H
#define GYROSENSOR_H

#include <QObject>
#include <QTimer>
#include <QDebug>

struct _GyroData{
    qreal AccelX;
    qreal AccelY;
    qreal AccelZ;
    qreal GyroX;
    qreal GyroY;
    qreal GyroZ;
};
typedef struct _GyroData GyroData;

inline void  print_GyroData(GyroData data){
    qDebug() <<"Accel: " << QString::number(data.AccelX) << QString::number(data.AccelY) << QString::number(data.AccelZ)
             << "\nGyro: " << QString::number(data.GyroX) << QString::number(data.GyroY) << QString::number(data.GyroZ);
}
class GyroSensorPrivate;
class GyroSensor : public QObject
{
    Q_OBJECT
    GyroSensorPrivate* pri = nullptr;
    QTimer timer;
private slots:
    void readsensorData();
public:
    explicit GyroSensor(QObject *parent = nullptr);
    void setPollingInterval(quint32 msec);
    virtual ~GyroSensor();
signals:
    void newSensorValue(GyroData newValue);// active singal sends new Value with singal
    void recivedSensorValue();// passive Singal indicates new Value can be read
public slots:

};

#endif // GYROSENSOR_H
