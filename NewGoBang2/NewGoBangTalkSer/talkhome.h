#ifndef TALKHOME_H
#define TALKHOME_H
#include<map>
#include<iostream>
#include"tcpcommunication.h"



class Home
{
public:
    Home();
    void addTcpCommunication(TcpCommunication* tcp);
    TcpCommunication* getPerCommunic(TcpCommunication *tcp);
    void removeTcpCommunication(TcpCommunication* tcp);
    bool judeHomeEmpty();
private:
    TcpCommunication *online_two_people[2];
};

struct TalkInfo
{
    enum
    {
        JoinHome,SendInfo,SendSuccess,SendFail,LevelHome
    };
    int  m_info_len; //信息的长度
    int  m_home_id;  //信息的id
    int  m_type;
    char m_info;     //信息的内容
};


class ChatHome
{
public:
    typedef int ID;
public:
    ChatHome();
    Home findIdHome(ID id);                          //根据id寻找想对应的home
    void addTcpCommunic(ID id,TcpCommunication *tcp);//在对应id
    void addHomeId(ID id);
    void removeHomeId(ID id);
    int  tcpLevel(TcpCommunication *tcp);
public:
    void addTcpToId(TcpCommunication *tcp,ID id);
    int  getId(TcpCommunication *tcp);
    void cleanHomeId(int id);
    void startSpy();
private:
    map<ID,Home> m_talk_que;
    map<long,ID> m_talk_id;
};

#endif // TALKHOME_H
