#include "iter.h"
#include<iostream>
#include<stdio.h>
#include"server.h"
#include"client.h"


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

void Iter::interacter()
{
    char com[20], *fileName;
    int count;

   while(1)
   {
        printf("\n(AutoSend):");
        fgets(com, sizeof(com), stdin);//
        transfStr(com, 1); //去掉开头结尾空白字符，变成全小写

        if (!strcmp(com, "list") ||
                !strcmp(com, "ls"))
        {

        }
        else if (!strcmp(com, "quit") ||
                 !strcmp(com, "q"))
        {

         // break;
        }
        else if (!strcmp(com, "refresh") ||
                 !strcmp(com, "rf"))
        {

        }
        else if (!strcmp(com, "talk") ||
                 !strcmp(com, "tk"))
        {

        }
        else if (!strcmp(com, "sendfile") ||
                 !strcmp(com, "sf"))
        {

        }
        else if (!strcmp(com, "getfile") ||
                 !strcmp(com, "gf"))
        {

        }
        else if (!strcmp(com, "ceaseSend") ||
                 !strcmp(com, "cs"))
        {

        }
        else if (!strcmp(com, "server") ||
                 !strcmp(com, "s"))
        {
         server = new Server;
         break;

        }
        else if (!strcmp(com, "client") ||
                 !strcmp(com, "c"))
        {
          client  = new Client;
          break;

        }
       else if (!strcmp(com, "help") ||
                 !strcmp(com, "h"))
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





