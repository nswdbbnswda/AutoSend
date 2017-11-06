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





    return a.exec();

}
