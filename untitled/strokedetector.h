#ifndef STROKEDETECTOR_H
#define STROKEDETECTOR_H

#include <QObject>

class StrokeDetector : public QObject
{
    Q_OBJECT
public:
    explicit StrokeDetector(QObject *parent = nullptr);

signals:

public slots:
};

#endif // STROKEDETECTOR_H