#ifndef FACTOARY_H
#define FACTOARY_H
#include<QObject>
#include"client.h"
#include <QCoreApplication>
#include<QCommandLineOption>
#include<QCommandLineParser>
#include"server.h"

class Factoary : public QObject{
    Q_OBJECT
public:
    explicit     Factoary();
    virtual      ~Factoary();
    virtual void setparser(QCoreApplication &a);
    std::queue<QString> GetFilePathQueue(QStringList paths);
    static std::queue<QString> pathlist;//路径队列
private:
    Server *server;
    Client *client;
    QStringList listpath;//list文件
    QStringList filePaths;

};


#endif // FACTOARY_H
