#pragma once
#include<iostream>
#include"./Server/explain.h"
#include"./Socket/tcplisten.h"
#include"./Socket/tcpcommunication.h"
#include"./Socket/epollloop.h"
#include<QDebug>
#include<QString>
using namespace std;


Explain explain;
void readCallBack(char* arry,int len,void *arg,TcpCommunication *tcpCon)
{
    if(len>0)
    {
        QByteArray arryv(arry);
        explain.dispatchServer(arryv,tcpCon);
    }
    else if(len==0)
    {
        cout<<"断开链接"<<__FUNCTION__<<__LINE__<<endl;
        ClientArg *arg=(ClientArg *)tcpCon->getReadArg();
        arg->clientDisconnect(tcpCon,explain.getHall());
    }
}

void writeCallBack(void *arg,TcpCommunication *tcpCon)
{
    tcpCon->changeStatus(TcpCommunication::Status::Read);
}

void acceptCallBack(TcpCommunication *tcpCon)
{
    tcpCon->setReadCallBack(readCallBack,nullptr);
    tcpCon->setWriteCallBack(writeCallBack,nullptr);
}

int main()
{
    EpollLoop loop(6666);
    loop.setReadcb  (readCallBack);
    loop.setWritecb (writeCallBack);
    loop.setAcceptcb(acceptCallBack);
    loop.setCanUse();

//    loop.initHeartTime(10);
//    loop.startAlarm();
    loop.loop();
    return 1;
}
