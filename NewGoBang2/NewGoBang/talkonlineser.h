#ifndef TALKONLINESER_H
#define TALKONLINESER_H
#include"head.h"
#include"sockclient.h"
#include"talkinfo.h"
#include<iostream>

using namespace std;


/*
    11.2日 增加的功能概要
    1:实现对战双方 的交流
        -实现细节:  开辟一个线程
                  去处理
*/
class talkOnlineSer
{
public:
    talkOnlineSer(string ip,int prot);

private:
    int        m_magic;//设置可用
    SockClient client;
};

#endif // TALKONLINESER_H
