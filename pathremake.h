#ifndef PATHREMAKE_H
#define PATHREMAKE_H
#include <QObject>
#include<queue>
#include<QFile>

class PathRemake : public QObject
{
    Q_OBJECT
public:
    explicit PathRemake();
    virtual  ~PathRemake();
    QString  GetSendFilePath(const QString &fullFilePath,const QString &dirPath);//获得文件路径不包括文件夹之前的部分 D:/DATA/1.txt -> DATA/1.txt
    bool     IsDirExist(const QString &path);//判断一个目录是否存在
private:
    QString  GetPrePath(const QString &_path);//D:/DATA/1.txt ->D:/DATA/  D:/temp -> D:/
public slots:

};

#endif // PATHREMAKE_H
