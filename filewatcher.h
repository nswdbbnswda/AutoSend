#ifndef FILEWATCHER_H
#define FILEWATCHER_H
#include"filebase.h"
#include<QDirIterator>
#include<QFileSystemWatcher>
#include<map>
#include<queue>
#include<QDebug>
#include<string>


//监视类
class FileWatcher : public FileBase{
    Q_OBJECT
public:
    FileWatcher(const std::string &s);
    virtual  ~FileWatcher();
    void  watchEverything();//监视一个路径下的所有的文件
    void GetFileList(const QString &path);
private slots:
     void findChangefile(const QString &path);//找到变化的文件，并存到队列中
signals:
     void newFile(QString);
private:
      QFileSystemWatcher myWatcher;
      std::map<QString,size_t> mymapCur;//当前目录下文件快照
      std::map<QString,size_t> mymapLast;//上一次目录下文件快照
public:
      static std::queue<QString> fileQueue;////文件队列
};

#endif // FILEWATCHER_H
