#ifndef CLIENT_H
#define CLIENT_H
#include<iostream>
#include<QFile>
#include<QObject>
#include<QTcpSocket>
#include<QFileInfo>
#include<QDir>
#include<QTime>
#include"autosend.h"
#include"pathremake.h"


class Client : QObject
{Q_OBJECT
public:
    explicit Client(const std::string strIpAddr,const std::string intputPort,const std::string _savePath);
    virtual~Client();
    enum TaskType{NEWTASK,BREAKTASK};

private:
    QTcpSocket  *m_pSocket;
    QByteArray  vTemp, xTemp,nTemp;
    char        *receiveName,*receiveFileNum;
    qint32      num,CurrentNum,TotalNum,LastBlock,nameLength;
    char        *ReceiveHead;
    qint32      port,Flag;
    qint64      FileLength;//文件总字节数
    std::string ipAddr;//IP地址
    std::string savePath;//储存文件的路径
    qint64      FileNumber;
    QFile       *logFile;
    bool        finishFlag;
    qint64      finishByte;//已经接收的总字节数
    qint64      cunrrentFinishByte;//当前接收的字节数
    QTime       time;//计时
    qint64      fileStartPos;//每次任务开始时，从最新一行开始对文件进行操作，这个变量记录了这个位置
    char        fileNameSeparator;//文件名分隔符
    int         taskType;//任务类型
    QByteArray  taskCode;//任务编号
    QByteArray  logContext;//日志内容
    QByteArray  breakFileName;//断点文件的名字
    qint64      breakFileLength;//断点文件已经接收的长度

protected:
    BreakPoint breakPoint;//断点
private:
    bool makePath(const QString &dirName);
    void connectToServer();//连接服务端
    bool findBreakPoint(const QByteArray & logArray);//从日志中找到断点

public slots:
    void receiveData();
    void lostConnection();//重新连接
    void showSpeed();//显示传输进度
    void responseTask();//响应任务
    bool sendIndexPos(const QString & name = "",qint64 pos = 0);//向服务端请求从索引处发送文件
signals:
    void dataComing();//数据来了
    void refresh();
    void taskCodeComing();//任务代号来了

};

#endif // CLIENT_H
