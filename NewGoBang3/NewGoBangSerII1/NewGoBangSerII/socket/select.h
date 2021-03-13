#ifndef EPOLLLOOP_H
#define EPOLLLOOP_H
#pragma once
#include"head.h"
#include"heartalarm.h"
#include"../business/server.h"
#include"sockarry.h"
#include"../business/explain.h"
#include<set>
#include<map>
#include<string>
#include<iostream>
#include <string.h>
#include <stdio.h>
#include <memory.h>
using namespace std;
#define PERALARM 400

class Select
{
public:
    Select(int port);
    ~Select();
    void loop();
    void startAlarm();
private:
    int  createSock(int prot);
    int  acceptCallBack();
    void readCallBack(int i);
    void writeCallBack(int i);
    void expCallBack(int i);
private:
    void doWrite(BackHead* tag,int cfd);
    void changeFdWrite(int cfd);
    void changeFdRead (int cfd);
    void changeFdRdWr(int cfd);
    void doAlarm();
private:
    void spy();
    void startSpy();
private:
    void removeFd(int cfd);
private:
    //处理explain返回的内容
    void dealExplain(char *info, int cfd);
    void dealPlayerGameOver(int fd);
    void dealPlayerLeftSer(int fd);

    void dealPlayerStopGame(int cfd);
private:
    atomic<int>         num;
    HeartAlarm          m_alarm;
    int                 lfd;              //监听套接字
    sockaddr_in         addr;             //地址
    fd_set              m_read;           //读集
    fd_set              m_write;          //写集
    fd_set              m_exp;            //异常集合
    map<int,SockArry>   mp;               //存放读出写入信息的集合
    map<int,HeartNode>  mp_heart;
    Explain             m_explain;
};
#endif // EPOLLLOOP_H
