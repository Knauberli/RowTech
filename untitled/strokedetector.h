#ifndef STROKEDETECTOR_H
#define STROKEDETECTOR_H

#include <QObject>
#include <QTime>
#include "ringstorage.h"
#define ds_ration 10
class StrokeDetector : public QObject
{
    Q_OBJECT
    QTime timer;
    RingStorage<qreal> *downSampleVlaues;
    enum StrokeState{

    };
    void rundetection();

private slots:
    void processnewvalue(qreal newValue);
public:
    explicit StrokeDetector(QObject *parent = nullptr);
    virtual ~StrokeDetector();
signals:
    void StrokeUpdate(quint8 spm);

public slots:

};

#endif // STROKEDETECTOR_H
