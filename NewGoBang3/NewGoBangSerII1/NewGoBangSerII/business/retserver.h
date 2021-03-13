#ifndef RETSENDBACK_H
#define RETSENDBACK_H
#include"server.h"
#include<iostream>
using namespace std;
//解析服务端返回的数据
class  Point;

struct BackHead
{
public:
    BackHead()
    {}
    BackHead(int type)
    {
        this->type=type;
    }
public:
    enum
    {youFrist,youSecond,fail,sendChessMan,endWaiting,timeOut,Talk};
    //你是先手    你是后手   成功    失败  获取到对方的落子点
    int  m_len=0; //信息长度
    int  m_num=0; //数据的数目
    bool m_tag=0; //sql语句是否执行成功
    char m_ch=0;  //数据内容
    int  type=-1;

};

//你是先手
class YouFrist:public BackHead
{
public:
    YouFrist():BackHead(BackHead::youFrist)
    {
        this->m_len=sizeof(YouFrist);
        this->id=0;
    }
    YouFrist(int _id):BackHead(BackHead::youFrist)
    {
        this->m_len=sizeof(YouFrist);
        this->id=_id;
    }
public:
    int id;
};

//你是后手
class YouSecond:public BackHead
{
public:
    YouSecond():BackHead(BackHead::youSecond)
    {
        this->m_len=sizeof(YouSecond);
    }
    YouSecond(int prot):BackHead(BackHead::youSecond)
    {
        this->m_len=sizeof(YouFrist);
        this->id=prot;
    }
public:
    int id;
};

class EndWaitRecv:public BackHead
{
public:
    EndWaitRecv():BackHead(BackHead::endWaiting)
    {
        this->m_len=sizeof(EndWaitRecv);
    }
};

//失败
class Fail:public BackHead
{
public:
    Fail():BackHead(BackHead::fail)
    {
        this->m_len=sizeof(Fail);
    }
};

//获取到
class SendChessMan:public BackHead
{
public:
    SendChessMan():BackHead(BackHead::sendChessMan)
    {
        this->m_len=sizeof(SendChessMan);
    }
    void setPoint(int x,int y)
    {
        point.x=x;
        point.y=y;
    }
    Point point;
};

class RetTimeOut:public BackHead
{
public:
    RetTimeOut():BackHead(BackHead::timeOut)
    {
        this->m_len=sizeof(RetTimeOut);
    }
};

class TalkInfoRet:public BackHead
{
public:
    TalkInfoRet(char *info,int str_len):BackHead(BackHead::Talk)
    {
        m_len=sizeof(TalkInfoRet);
        memcpy(m_info,info,strlen(info));
        m_len=str_len;
    }
public:
    char m_info[1024];     //信息的内容
    int  m_len;
    int  m_home_id;
};
#endif // RETSENDBACK_H