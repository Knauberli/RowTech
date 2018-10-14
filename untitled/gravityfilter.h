#ifndef GRAVITYFILTER_H
#define GRAVITYFILTER_H

#include <QObject>
#include "gyrosensor.h"

class GravityFilter : public QObject
{
    Q_OBJECT
    GyroData gravity; // just a storage for delaying data
    qreal alpha = 0.8; // should be smaller than 1 for stability
    qreal cuttoff = 1; // Cuttoff Frequency
    qreal deltatime = 1.0/200; // Event delivery rate

    void calculateAlpth();
public:
    explicit GravityFilter(QObject *parent = nullptr);
    void setCuttoff(qreal Cuttoff);
    void setSampleFrequency(qreal Deltatime);
signals:

public slots:
    void filterGravity(GyroData& data);
};

#endif // GRAVITYFILTER_H
