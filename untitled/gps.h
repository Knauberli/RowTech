#ifndef GPS_H
#define GPS_H

#include <QObject>
#include <QMutex>
#include <QTimer>
enum Orinetation_{NW,NE,SW,SE};
typedef enum Orinetation_ Orinetation;
class QFile;

class GPS : public QObject
{
    Q_OBJECT

    /* Enum for indicating the mount on earth E= East
     * W = West, N = North, S = South
    */
    enum Orinetation_ m_orinet;
    qreal m_longintude = 0.0;
    qreal m_latidude = 0.0;
    QMutex lock_values;
    QString m_devicepath;
    QFile* device = nullptr;
    QTimer timer;
    void setupTimeing();
private slots:
    void parseData();
public:
    explicit GPS(QObject *parent = nullptr);
    GPS(QString path, QObject *parent);
    virtual ~GPS();
    qreal getLatitude();
    qreal getLongitude();
    Orinetation getOrientation();
signals:
    void operate();

public slots:
};

#endif // GPS_H
