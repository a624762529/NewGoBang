#include "talkhome.h"

Home::Home()
{
    online_two_people[0]=nullptr;
    online_two_people[1]=nullptr;
}

void Home::addTcpCommunication(TcpCommunication* tcp)
{
    if(tcp==online_two_people[0]||tcp==online_two_people[1])
    {
        return;
    }
    if(online_two_people[0]==nullptr)
    {
        online_two_people[0]=tcp;
    }
    else if(online_two_people[1]==nullptr)
    {
        online_two_people[1]=tcp;
    }
}

void Home::removeTcpCommunication(TcpCommunication* tcp)
{
    if(online_two_people[0]==tcp)
    {
        online_two_people[0]=nullptr;
    }
    else if(online_two_people[1]==tcp)
    {
        online_two_people[1]=nullptr;
    }
    return;
}

bool Home::judeHomeEmpty()
{
    return (online_two_people[0]==nullptr
            &&
            online_two_people[1]==nullptr);
}

TcpCommunication* Home::getPerCommunic(TcpCommunication *tcp)
{
    if(online_two_people[0]==tcp)
    {
        return online_two_people[1];
    }
    else
    {
        return online_two_people[0];
    }
}

ChatHome::ChatHome()
{
    thread th(&ChatHome::startSpy,this);
    th.detach();
}

void ChatHome::startSpy()
{
    while (true)
    {
        cout<< m_talk_que.size()<<endl;;
        cout<< m_talk_id.size()<<endl;
        sleep(4);
    }
}

Home ChatHome::findIdHome(ID id)
{
    return m_talk_que.find(id)->second;
}

void ChatHome::addTcpCommunic(ID id,TcpCommunication *tcp)
{
    m_talk_que.find(id)->second.addTcpCommunication(tcp);
    addTcpToId(tcp,id);
}

void ChatHome::addHomeId(ID id)
{
    if(m_talk_que.find(id)==m_talk_que.end())
    {
       m_talk_que.insert(map<ID,Home>::value_type(id,Home()));
    }
    return;
}

int ChatHome::tcpLevel(TcpCommunication *tcp)
{
    //获取homeid
    int id=getId(tcp);
    if(id==-1)
    {
        return id;
    }
    m_talk_que.find(id)->second.removeTcpCommunication(tcp);
    m_talk_id.erase((long)tcp);
    return 1;
}


void ChatHome::addTcpToId(TcpCommunication *tcp,ID id)
{
    m_talk_id.insert(map<long,ID>::value_type((long)tcp,id));
}

int  ChatHome::getId(TcpCommunication *tcp)
{
    map<long,ID>::iterator id=m_talk_id.find((long)tcp);
    if(id !=m_talk_id.end())
    {
        return id->second;
    }
    else
    {
        return -1;
    }
}

void ChatHome::cleanHomeId(int id)
{
    m_talk_que.erase(id);
}
