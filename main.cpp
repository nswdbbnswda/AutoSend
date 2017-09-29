#include <QCoreApplication>
#include"server.h"
#include"iter.h"
#include"client.h"
#include"server.h"
#include"filebase.h"
#include"filewatcher.h"
#include"sender.h"
#include<iostream>
#include<Windows.h>
using namespace std;

#include <QTextStream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Iter iter;
    iter.interacter(argc,argv);//启动用户交互线程

 /*   QByteArray fileContext("Hello");
    QFile logFile(QCoreApplication::applicationDirPath()+"/test.log");//建立日志文件

    logFile.open(QIODevice::ReadWrite| QIODevice::Append);//以追加的方式
   // logFile.seek(1);//移动文件指针到位置1
    logFile.write(fileContext);
    logFile.flush();
    logFile.close()*/;







    return a.exec();

}
