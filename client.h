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
    explicit Client(const std::string strIpAddr,const std::string intputPort);
    virtual~Client();
private:
    QTcpSocket *m_pSocket;
    QByteArray vTemp, xTemp,nTemp;
    char *ReceiveName,*ReceiveFileNum;
    qint32 num,CurrentNum,TotalNum,LastBlock,NameLength;
    char *ReceiveHead;
    qint32 port,Flag,TotalByte,WrittenByte;
    std::string ipAddr;
    qint64 FileNumber;
    QFile *logFile;
private:
    bool  KoPath(const QString &dirName);
public slots:
    void ReceiveData();
    void UpProgress();
    void test();
signals:
    void DataComing();
    void DataWritten();

};

#endif // CLIENT_H
