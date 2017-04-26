#ifndef LIB_H
#define LIB_H
#include<iostream>
#include<QString>

//存储文件路径和文件名的结构体
struct openFileStruct
{
    QString filePath; //文件路径
    QString fileName; //文件名
};

struct clientInfo   //客户端信息
{
    QString ip;     //ip
    int state;      //状态
    QString id;     //id
};



const quint8 sendtype_file = 0;    //发送类型是文件
const quint8 sendtype_start_test = 10;    //发送类型是开始测试
const quint8 sendtype_msg = 20;    //发送类型是消息
const quint16 filetype_list = 0;    //文件类型为列表
const quint16 filetype_wavfile = 1;    //文件类型为wav文件
const QString clientStatus[7] =    //客户端状态
    {QObject::tr("Unconnected"),QObject::tr("HostLookup"),QObject::tr("Connecting"),
        QObject::tr("Connected"),QObject::tr("Bound"),QObject::tr("Listening"),QObject::tr("Closing")};


#endif // LIB_H
