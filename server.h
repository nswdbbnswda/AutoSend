#ifndef SERVER_H
#define SERVER_H
#include<QTcpServer>
#include<QTcpSocket>
#include<QObject>
#include<iostream>
#include<string>
#include<QFile>
#include<QDir>
#include<QFile>
#include<QDebug>
#include<QDateTime>
#include<QDirIterator>
#include<queue>
#include<QFileSystemWatcher>



class Server : QObject
{  Q_OBJECT
public:
    explicit Server();
  void WatchEvery(const QString &path);
    ~Server();

private:
    QTcpServer *server;//监听套接字
    QTcpSocket *m_Socket;//连接套接字
    std::string path,dirpath;
    qint32 FileLength;//文件的长度
    qint32 BlockNum;//整块数量
    qint32 TotalByte,temp,PathLength;
    qint32 Num2,Num3,Num4,TotalNum;
    qint32 LastBlock;
    char  *SendBuffer;
    char  *SendPath;
    qint32 lenth;
    char *FileNum;


    std::map<QString,size_t> mymapCur;
    std::map<QString,size_t> mymapLast;
private:
     std::queue<QString> *Fileque;////文件队列
     std::queue<QString> *Pathque;//路径队列
     void  GetFileList(const QString &path);//获得目录下的所有文件
      QFileSystemWatcher myWatcher;

signals:
    void SendData();//发送数据
private slots:
    void newConnectionSlot();//对新的TCP连接进行处理
    void  UpProgress();//显示进度
    void showMessage(const QString &path);

};

#endif // SERVER_H
