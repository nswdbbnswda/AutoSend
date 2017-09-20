#ifndef CLIENT_H
#define CLIENT_H
#include<iostream>
#include<QFile>
#include<QObject>
#include<QTcpSocket>
#include<QFileInfo>
#include<QDir>
#include<QTime>


class Client : QObject
{Q_OBJECT
public:
    explicit Client(const std::string strIpAddr,const std::string intputPort);
    virtual~Client();
private:
    QTcpSocket  *m_pSocket;
    QByteArray  vTemp, xTemp,nTemp;
    char        *receiveName,*receiveFileNum;
    qint32      num,CurrentNum,TotalNum,LastBlock,nameLength;
    char        *ReceiveHead;
    qint32      port,Flag;
    qint64      FileLength;//文件总字节数
    std::string ipAddr;
    qint64      FileNumber;
    QFile       *logFile;
    bool        finishFlag;
    qint64      finishByte;//已经接收的总字节数
    qint64      cunrrentFinishByte;//当前接收的字节数
    QTime       time;//计时
private:
    bool KoPath(const QString &dirName);
    void connectToServer();//连接服务端
public slots:
    void ReceiveData();
    void lostConnection();//重新连接
    void showSpeed();//显示传输进度
    void test();//测试 找出CPU占用过高的BUG
signals:
    void DataComing();
    void refresh();
};

#endif // CLIENT_H
