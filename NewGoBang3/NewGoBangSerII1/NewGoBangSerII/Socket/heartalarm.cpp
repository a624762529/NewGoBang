#include "heartalarm.h"

HeartAlarm::HeartAlarm()
{
    int ret=pipe(fd);
    if(ret==-1)
    {
        perror("pipe open error");
    }
    m_start=false;
    m_sleep_alarm=999;
}

HeartAlarm::HeartAlarm(int alarm)
{
    m_sleep_alarm=alarm;
    int ret=pipe(fd);
    if(ret==-1)
    {
        perror("pipe open error");
    }
    m_start=false;

}

void HeartAlarm::PushNode(HeartNode node)
{
    if((*node.m_inslot)==0)
    {
        m_alarmSlot[0].push_back(node);
        return;
    }
}

vector<int> HeartAlarm::move()
{
    //获取尾部全部通讯文件描述符
    vector<int> vec;
    for(vector<HeartNode>::iterator it=m_alarmSlot[3].begin();
        it!=m_alarmSlot[3].end();it++)
    {
        if((*it->m_inslot)==0)
        {
            vec.push_back((*it).m_fd);
        }
        else
        {
            (*it->m_inslot)=0;
            m_alarmSlot[0].push_back(*it);
        }
    }
    //移动
    for(int i=3;i>0;i--)
    {
        swap(m_alarmSlot[i],m_alarmSlot[i-1]);
    }
    m_alarmSlot[0].clear();

    return vec;
}

void HeartAlarm::startAlarm()
{
    if(!m_start)
    {
         m_start=true;
        thread th(&HeartAlarm::writeFd,this);
        th.detach();

    }
}

void HeartAlarm::writeFd()
{
    signal(SIGPIPE,SIG_IGN);
    char ch='a';
    while (m_start)
    {
        sleep(m_sleep_alarm);
        int ret=write(fd[1],&ch,1);
        if(ret==-1)
        {
            if(errno==EAGAIN)
            {
                continue;
            }
            else
            {
                perror("error:");
                throw bad_alloc();
            }
        }
    }
}

//获取读文件描述符
int HeartAlarm::getReadFd()
{
    return fd[0];
}

void HeartAlarm::setAlarm(int per_al)
{
    m_sleep_alarm=per_al;
}
