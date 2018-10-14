#ifndef MA_FILTER_H
#define MA_FILTER_H

#include <QObject>
#include "gyrosensor.h"
class MA_Filter_private;
class MA_Filter : public QObject
{
    Q_OBJECT
    MA_Filter_private* priv;
    MA_Filter_private* g_comprassion;
    GyroData g_estimation ;
    void filter_layer_1(GyroData &data);
    void filter_g(GyroData &data);
public:
    explicit MA_Filter(QObject *parent = nullptr);
signals:
    void updatedOutput(qreal value);
public slots:
    void updateInput(GyroData& data);
};

#endif // MA_FILTER_H
