#include <QCoreApplication>
#include"server.h"
#include"iter.h"
#include"client.h"
#include"server.h"



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Iter fuck;
    fuck.interacter();//启动用户交互线程
    return a.exec();
}
