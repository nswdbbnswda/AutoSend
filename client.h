#ifndef CLIENT_H
#define CLIENT_H
#include<iostream>
#include<QFile>
#include<QObject>
#include<QTcpSocket>
#include<QFileInfo>
#include<QDir>


class Client : QObject
{Q_OBJECT
public:
   explicit Client();
    ~Client();

private:
     QTcpSocket *m_pSocket;
     QByteArray vTemp, xTemp,nTemp;
     char *ReceiveName,*ReceiveFileNum;
     qint32 num,CurrentNum,TotalNum,LastBlock,NameLength;
     char *ReceiveHead;
     qint32 port,Flag,TotalByte,WrittenByte;
     std::string ipAddr;
     qint64 FileNumber;
private:
     bool  Client::KoPath(const QString &dirName);//搞定路径
public slots:
    void ReceiveData();
    void UpProgress();
    void test();
signals:
   void  DataComing();
   void DataWritten();
};

#endif // CLIENT_H
