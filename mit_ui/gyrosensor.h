#ifndef GYROSENSOR_H
#define GYROSENSOR_H

#include <QObject>
#include <QTimer>
struct _GyroData{
    qreal AccelX;
    qreal AccelY;
    qreal AccelZ;
    qreal GyroX;
    qreal GyroY;
    qreal GyroZ;
};
typedef struct _GyroData GyroData;

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
