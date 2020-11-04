#ifndef HEARTALARM_H
#define HEARTALARM_H
#include"head.h"
#include<iostream>

using namespace std;

class HeartNode
{
public:
    HeartNode(int fd)
    {
        m_fd=fd;
        m_inslot=new int(0);
    }

    void Add()
    {
        (*m_inslot)++;
    }

    void destory()
    {
        m_fd=0;
        if(m_inslot)
        {
            free(m_inslot);
        }
        m_inslot=nullptr;
    }
public:
    int m_fd;
    int *m_inslot=nullptr;
};

class HeartAlarm
{
public:
    HeartAlarm();
    HeartAlarm(int alarm);
    void        PushNode(HeartNode node);
    vector<int> move();
    void        startAlarm();
    int         getReadFd();
    void        setAlarm(int per_al);
public:
    vector<HeartNode> m_alarmSlot[4];
private:
    void writeFd();
private:
    int fd[2];
    int  m_sleep_alarm;
    bool m_start;
};

#endif // HEARTALARM_H
