#ifndef GPS_H
#define GPS_H

#include <QObject>

class GPS : public QObject
{
    Q_OBJECT
public:
    explicit GPS(QObject *parent = nullptr);

signals:

public slots:
};

#endif // GPS_H