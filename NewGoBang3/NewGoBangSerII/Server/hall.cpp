#include "hall.h"

Hall::Hall()
{
    memset(homeTag,0,sizeof(homeTag));
}

ClientArg* Hall::addTcpCommunciationToWaitQue(TcpCommunication *con)
{
    vec.push_back(con);
    if(vec.size()>=2)
    {
        int ret=getHomeId();
        if(ret==-1)
        {
            //没有房间了
        }
        else
        {
            TcpCommunication *first=*vec.begin();
            vec.erase(vec.begin());

            TcpCommunication *second=*(vec.begin()+1);
            vec.erase(vec.begin());

            ClientArg *arg=new ClientArg();
            arg->addTcpCommunication(first);
            arg->addTcpCommunication(second);

            first->setReadArg(arg);
            first->setWriteArg(arg);
            second->setReadArg(arg);
            second->setWriteArg(arg);
            return arg;
        }
    }
    return nullptr;
}

int Hall::getHomeId()
{
    for(int i=0;i<1024;i++)
    {
        if(homeTag[i]==0)
        {
            homeTag[i]=1;
            return i;
        }
    }
    return -1;
}

int Hall::rollBackId(int id)
{
    homeTag[id]=0;
}

//将tco链接从等待队列中移除
bool Hall::removeInQueue(TcpCommunication *con)
{
    for(vector<TcpCommunication *>::iterator it=vec.begin();it!=vec.end();it++)
    {
        if(*it==con)
        {
            vec.erase(it);
            return true;
        }
    }
    return false;
}
