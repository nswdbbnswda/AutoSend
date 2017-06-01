#ifndef FILEWATCHER_H
#define FILEWATCHER_H
#include"filebase.h"
#include<QDirIterator>
#include<QFileSystemWatcher>
#include<map>
#include<queue>
#include<QDebug>
#include<string>


//监视类 单例模式
class FileWatcher : public FileBase{
    Q_OBJECT
public:
    static FileWatcher *getInstance(const std::string &s)//静态成员函数,谁都可以调用  调用后会获得一个监视器对象指针
    {
        static FileWatcher *m_pInstance;
        if(m_pInstance == NULL){
            m_pInstance = new  FileWatcher(s);
        }           return m_pInstance;

    }

    void  watchEverything();//监视一个路径下的所有的文件
    void GetFileList(const QString &path,std::map<QString,size_t> &saveMap);
    void GetFileList(const QString &path,std::queue<QString> &saveQueue);
    virtual  ~FileWatcher();

 private:
    FileWatcher(const std::string &s);
    FileWatcher() = default;

private slots:
     void findChangefile(const QString &path);//找到变化的文件，并存到队列中

private:
      QFileSystemWatcher myWatcher;
      std::map<QString,size_t> mymapCur;//当前目录下文件快照
      std::map<QString,size_t> mymapLast;//上一次目录下文件快照

public:
      static std::queue<QString> fileQueue;////文件队列
signals:
      void fileChange(QString);
};

#endif // FILEWATCHER_H
