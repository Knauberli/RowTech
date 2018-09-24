#ifndef MA_FILTER_PRIVATE_H
#define MA_FILTER_PRIVATE_H
#include <QObject>
#include <QVector>
#include "ringstorage.h"
#include "gyrosensor.h"
class MA_Filter_private : public QObject
{
    Q_OBJECT
    RingStorage<qreal> *databuffer = nullptr;
    QVector<qreal> koeff;
    void read_koeff(QString path);
public:
    explicit MA_Filter_private(QObject *parent=nullptr);
    MA_Filter_private(QString keffFilePath,QObject *parent=nullptr);
    void setKoeffPath(QString path);
    qreal processNewValue(qreal data);
    virtual ~MA_Filter_private();
};
#endif // MA_FILTER_PRIVATE_H
