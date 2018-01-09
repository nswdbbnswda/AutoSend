#ifndef HEARTBEAT_H
#define HEARTBEAT_H
#include<QObject>
#include<QDebug>
#include<QTcpSocket>
#include<QTimer>
#include<QThread>
#include<QMutex>
#include <QCoreApplication>
class HeartBeat : public QObject
{
    Q_OBJECT
public:
    explicit HeartBeat();
    virtual  ~HeartBeat();
public slots:
    void Burn(int desc);
    void heartTime();
    void startTimer();
private:
    QTcpSocket *pSocket;
    QTimer *myTimer;
    QMutex *lock;

};

#endif // HEARTBEAT_H
