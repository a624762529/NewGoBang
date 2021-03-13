#ifndef SERVER_H
#define SERVER_H
#include"head.h"

using namespace std;

struct Point
{
    int x=-1;
    int y=-1;
};
//客户端给服务端发送的数据信息的头部;
class SendPackImpl
{
public:
    enum{
        //加入房间和其他玩家玩耍   登录  注册    发送棋子
        playWithOther,         load, login, sendChess,
        //超时                  赢了          放弃等待        对战双方之间交流
        timeOut,               success,     giveUpWating   ,Talk,
        //加入聊天房间
        JoinTalkHome
    };


    SendPackImpl()
    {
        m_send_type=-1;
        m_info_len=-1;
    }
    SendPackImpl(int type)
    {
        m_send_type=type;
        m_info_len=-1;
    }
public:
    char m_username[20]{0};
    int m_send_type;//发送类型
    int m_info_len; //信息的长度
};

class PlayWithOther:public SendPackImpl
{
public:
    PlayWithOther():SendPackImpl(SendPackImpl::playWithOther)
    {
        m_info_len=sizeof(playWithOther);
    }
};

class Load:public SendPackImpl
{
public:
    Load():
        SendPackImpl(SendPackImpl::load)
    {
        m_info_len=sizeof(load);
    }
};

class Login:public SendPackImpl
{
public:
    Login():
        SendPackImpl(SendPackImpl::login)
    {
        m_info_len=sizeof(login);
    }
};

class SendChess:public SendPackImpl
{
public:
    SendChess():
        SendPackImpl(SendPackImpl::sendChess)
    {
        m_info_len=sizeof(sendChess);
    }
    void setPoint(int x,int y)
    {
        point.x=x;
        point.y=y;
    }
public:
    Point point;
};

class TimeOut:public SendPackImpl
{
public:
    TimeOut():
        SendPackImpl(SendPackImpl::timeOut)
    {
        m_info_len=sizeof(timeOut);
    }
};

class Success:public SendPackImpl
{
public:
    Success():SendPackImpl(SendPackImpl::success)
    {
        m_info_len=sizeof(Success);
    }
};

class GiveUpWating:public SendPackImpl
{
public:
    GiveUpWating():SendPackImpl(SendPackImpl::giveUpWating)
    {
        m_info_len=sizeof(GiveUpWating);
    }
};


//1.13更新的内容　增加对战双方的交流
class TalkInfo:public SendPackImpl
{
public:
    TalkInfo(char *info,int str_len):SendPackImpl(SendPackImpl::Talk)
    {
        m_info_len=sizeof(TalkInfo);
        memcpy(m_info,info,strlen(info));
        m_len=str_len;
    }
public:
    char m_info[1024];     //信息的内容
    int  m_len;
    int  m_home_id;
};

class JoinTalkHome:public SendPackImpl
{
public:
    JoinTalkHome():SendPackImpl(SendPackImpl::Talk)
    {
        m_info_len=sizeof(TalkInfo);
    }
     int home_id;
};
#endif // SERVER_H
