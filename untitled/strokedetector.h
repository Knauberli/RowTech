#ifndef STROKEDETECTOR_H
#define STROKEDETECTOR_H

#include <QObject>
#include <QTime>
class StrokeDetector : public QObject
{
    Q_OBJECT
    QTime timer;
    enum StrokeState{

    };

private slots:
    void processnewvalue(qreal newValue);
public:
    explicit StrokeDetector(QObject *parent = nullptr);

signals:
    void StrokeUpdate(quint8 spm);

public slots:

};

#endif // STROKEDETECTOR_H
