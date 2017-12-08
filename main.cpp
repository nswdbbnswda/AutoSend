#include <QCoreApplication>
#include"server.h"
#include"client.h"
#include"server.h"
#include"filewatcher.h"
#include"sender.h"
#include<iostream>
#include <QTextStream>
#include"Factoary.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Factoary app;
    app.setparser(a);





    return a.exec();
}


