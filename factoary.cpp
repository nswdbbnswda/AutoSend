#include"Factoary.h"




std::queue<QString> Factoary::pathlist;//工厂内的路径队列

Factoary::Factoary()
{

}

Factoary::~Factoary()
{
    if(server){ delete server;}
    if(client){delete client;}
}




//设置解析参数
void Factoary::setparser(QCoreApplication &a)
{
    //定义命令行对象
    QCommandLineOption op1("s");// 短名称，无参数 s表示发送
    QCommandLineOption op2("c");// 短名称，无参数 c表示接收
    QCommandLineOption op3("i","ip addr","ip");// 长名称，有参数，无默认值，IP地址
    QCommandLineOption op4("list", "enter a number of file list file path", "listpath"); //list后面跟跟若干个路径列表文件的地址
    QCommandLineOption op5("d", "file path or dir path", "path");// 短名称，有参数，无默认值  路径
    QCommandLineOption op6("p","socket port","port","5000");// 短名称，有参数，有默认值  默认端口是5000
    //命令行解析器
    QCommandLineParser parser;
    parser.addOption(op1);
    parser.addOption(op2);
    parser.addOption(op3);
    parser.addOption(op4);
    parser.addOption(op5);
    parser.addOption(op6);
    parser.process(a);

    if((parser.isSet(op1) && parser.isSet(op2))||(!parser.isSet(op2) && !parser.isSet(op1))){//程序启动无效
        qDebug()<<"无效命令行参数";
        exit(0);//退出程序
    }

    if(parser.isSet(op1) && parser.isSet(op5)){//添加手动输入的路径
        pathlist =  GetFilePathQueue(parser.values(op5));//将路径都存放到路径队列中
    }
    if(parser.isSet(op4)){//如果使用者想通过提供一个路径列表文件来获得路径列表

        QStringList tmpPath = parser.values(op4);//获得list文件地址
        QStringList::Iterator it = tmpPath.begin(),itend = tmpPath.end();//定义两个迭代器
        while(it != itend){
            qDebug()<<"正在从"<<*it<<"文件中读取路径...";
            //判断每list文件的路径合法性
            QByteArray dirBuf;
            QFileInfo fileInfo(*it);
            if(!fileInfo.exists()){ qDebug()<<"路径不合法!"; return;}//判断list路径有效性
            if(!fileInfo.isFile()){qDebug()<<*it<<"文件不存在"; return;}//不是一个文件路径


            //打开list文件
            QFile listFile(*it);//创建list文件对象
            listFile.open(QIODevice::ReadOnly);//以只读的方式打开这个list文件


            //把list文件中的路径保存起来
            while(true){
                dirBuf = listFile.readLine();//逐行读取路径
                if(dirBuf.contains('\n')){dirBuf.remove(dirBuf.indexOf('\n'),1);}//把\n字符删了
                if(dirBuf.contains('\r')){dirBuf.remove(dirBuf.indexOf('\r'),1);}//把\n字符删了
                //if(!dirBuf.isEmpty())qDebug()<<dirBuf;//如果读到了字符串就显示一下内容
                if(dirBuf.isNull()){break;}//什么都没有读到就不再读取该文件了(认为读完了所有行)(其实有BUG,如果各行之间有空行情况没处理)
                QString tmp = dirBuf;
                QFileInfo inerFilePath(tmp);//list文件中的文件中每一行内容
                if(!inerFilePath.exists()){qDebug()<<*it<<"列表中的"<<tmp<<"路径不合法"; return;}//判断每一行的合法性
                pathlist.push(tmp);
                qDebug()<<tmp;
            }

            //关闭list文件
            listFile.close();
            ++it;//切换到下一个list文件
        }
    }

    //生成对象
    if(parser.isSet(op1) ){server = new Server(parser.value(op6).toStdString());}//创建服务端 //传入端口号
    else{ client = new Client(parser.value(op3).toStdString(),parser.value(op6).toStdString(),parser.value(op5).toStdString());}//创建客户端
}


//输入一个QStringList路径串，返回一个文件队列
std::queue<QString> Factoary::GetFilePathQueue(QStringList paths)
{
    std::queue<QString> plst;
    try{
        QStringList::Iterator it = paths.begin(),itend = paths.end();//定义两个迭代器
        //把QStringList中的路径都添加到路径队列中
        while(it != itend){
            QFileInfo inerFilePath(*it);//list文件中的文件中每一行内容
            if(!inerFilePath.exists()){throw ("路径不合法");}
            plst.push(*it);
            qDebug()<<*it;
            ++it;
        }
    }catch(QString ex){
        qDebug()<<ex;
        exit(0);
    }
    return plst;
}








