#include "iter.h"
#include<iostream>
#include<stdio.h>
#include"server.h"
#include"client.h"
#include<iostream>
using namespace  std;


Iter::Iter()
{


}

Iter::~Iter()
{
    if(server){
        delete server;
        server = NULL;
    }
    if(client){
        delete server;
        server = NULL;
    }
}

void Iter::interacter(int iArgc ,char **p_cCmd)
{
   // std::cout<<*(*p_cCmd + 1)<<std::endl;//显示第一个字符串

    char com[20], *fileName;
    int count;
    if(iArgc <=1){cout<<"Lack of param!"<<endl; return ;}
    if(NULL == p_cCmd){ printf("No available param from argv!");  return;}
  // while(1)
   {
        printf("\n(AutoSend):");
        //fgets(com, sizeof(com), stdin);//
        //transfStr(com, 1); //去掉开头结尾空白字符，变成全小写

        if (!strcmp(*(p_cCmd+1), "list") ||
                !strcmp(*(p_cCmd+1), "ls"))
        {

        }
        else if (!strcmp(*(p_cCmd+1), "quit") ||
                 !strcmp(*(p_cCmd+1), "q"))
        {

        }
        else if (!strcmp(*(p_cCmd+1), "refresh") ||
                 !strcmp(*(p_cCmd+1), "rf"))
        {

        }
        else if (!strcmp(*(p_cCmd+1), "talk") ||
                 !strcmp(*(p_cCmd+1), "tk"))
        {

        }
        else if (!strcmp(*(p_cCmd+1), "sendfile") ||
                 !strcmp(*(p_cCmd+1), "sf"))
        {

        }
        else if (!strcmp(*(p_cCmd+1), "getfile") ||
                 !strcmp(*(p_cCmd+1), "gf"))
        {

        }
        else if (!strcmp(*(p_cCmd+1), "ceaseSend") ||
                 !strcmp(*(p_cCmd+1), "cs"))
        {

        }
        else if (!strcmp(*(p_cCmd+1), "server") ||
                 !strcmp(*(p_cCmd+1), "s"))
        {
         Server::dirpath = *(p_cCmd+3);

         server = new Server(*(p_cCmd+2));//把端口号传给Server类

        }
        else if (!strcmp(*(p_cCmd+1), "client") ||
                 !strcmp(*(p_cCmd+1), "c"))
        {
          client  = new Client(*(p_cCmd+2),*(p_cCmd+3),*(p_cCmd + 4));//client 127.0.0.1 5000



        }
       else if (!strcmp(*(p_cCmd+1), "help") ||
                 !strcmp(*(p_cCmd+1), "h"))
            printf(IMHELP);
     }
}




void Iter::transfStr(char *dest, int flag)
{
    char *ptr;
    int len;
    ptr = dest;
    while (isspace(*ptr))
        ptr++;
    len = strlen(ptr);
    if (ptr > dest)
        memmove(dest, ptr, len+1);
    ptr = dest+len-1;
    while (isspace(*ptr))
        ptr--;
    *(ptr+1) = '\0';
    ptr = dest;
    if (flag == 1)
        while (*ptr!='\0'){
            *ptr = tolower(*ptr);
            ptr++;
        }
}











