#ifndef _GYROSENSORPRIVATE_H
#define _GYROSENSORPRIVATE_H

#include <QObject>
#include <QTimer>
// use Burst Read from the SensoR

struct _GyroData;
class GravityFilter;
class GyroSensorPrivate :public QObject
{
    Q_OBJECT
    QString filePath;
    QTimer timer_;
    int filedescriptor;
    GravityFilter* gv;
    enum _SensorState{
        READY,
        ERROR,
        NOTINITILZED,
        CLOSED
    };
    int state_ = NOTINITILZED;
    typedef enum _SensorState SensorState;

    //private Functions
    uchar getRegisterValue(uchar RegisterNumber);
    uchar* readBurstDataFifo();
    double AccelFactor;
    double GyroFactor;
    void calculateRelativeValues(struct _GyroData *data);
    void SetupI2C();
public:
    explicit GyroSensorPrivate(QObject* parent=nullptr);
    GyroSensorPrivate(QString devFilepath,QObject *parent =nullptr);
    virtual ~GyroSensorPrivate();
    bool isReady();
    void closeBus();
    void reopenBus();
    _GyroData readData();
};

#endif // _GYROSENSORPRIVATE_H
