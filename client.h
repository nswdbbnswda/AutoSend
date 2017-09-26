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
    qint64     FileLength;//文件总字节数
    std::string ipAddr;
    qint64      FileNumber;
    QFile       *logFile;
    bool        finishFlag;
    qint64       finishByte;//已经接收的总字节数
    qint64      cunrrentFinishByte;//当前接收的字节数
    QTime       time;//计时
    qint64      fileStartPos;//每次任务开始时，从最新一行开始对文件进行操作，这个变量记录了这个位置
private:
    bool KoPath(const QString &dirName);
    void connectToServer();//连接服务端
public slots:
    void receiveData();
    void lostConnection();//重新连接
    void showSpeed();//显示传输进度
    void receiveFileList();//接收文件清单

signals:
    void dataComing();
    void refresh();
};

#endif // CLIENT_H
