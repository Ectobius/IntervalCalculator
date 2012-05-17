#ifndef SIGNALIZER_H
#define SIGNALIZER_H

#include <QObject>

class Signalizer : public QObject
{
    Q_OBJECT
public:
    void sendSignal();

signals:
    void signal();

};

#endif // SIGNALIZER_H
