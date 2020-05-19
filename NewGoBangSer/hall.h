#ifndef HALL_H
#define HALL_H
#include<iostream>
#include<set>
#include<vector>
#include<map>
#include<algorithm>
using namespace std;

class Home
{
public:
    Home() {}
    int getPerFd(int fd)
    {
        if(cfd1==fd)
        {
            return cfd2;
        }
        else
        {
            return cfd1;
        }
    }
public:
    int cfd1=-1;
    int cfd2=-1;
};


struct RetBack
{
    Home home;
    int  id=-1;
    bool is_success=false;
};

typedef int  Id;

class Hall
{
public:
    pair<bool,Id> getFreeHomeId()
    {
        //要是房间够用 pair first为true  否则false
        for(int i=0;i<100;i++)
        {
            if(arry[i]==0)
            {
                arry[i]=1;
                return pair<bool,int>(true,i);
            }
        }
        return pair<bool,int>(false,-1);
    }

    void retBackHomeId(int id)
    {
        arry[id]=0;
    }

    void removeFromMapFdToId(int fd)
    {
        //移除fd对应的房间号
        m_fd_to_id.erase(fd);
    }

    bool addPeople(int fd)
    {
        auto ret=find(m_waitque.begin(),m_waitque.end(),fd);
        if(ret==m_waitque.end())
        {
            m_waitque.push_back(fd);
            return true;
        }
        return false;
    }

    RetBack addHome()
    {
        RetBack retv;
        auto ret=getFreeHomeId();
        if(ret.first==false)
        {
            return retv;
        }
        if(m_waitque.size()>=2)
        {
            int fd1=*m_waitque.begin();
            int fd2=*(m_waitque.begin()+1);
            m_waitque.erase(m_waitque.begin());
            m_waitque.erase(m_waitque.begin());
            Home home;
            home.cfd1=fd1;
            home.cfd2=fd2;

            m_fd_to_id.insert(pair<int,Id>(fd1,ret.second));
            m_fd_to_id.insert(pair<int,Id>(fd2,ret.second));

            m_id_to_fd.insert(pair<int,Home>(ret.second,home));
            retv.home=home;
            retv.id=ret.second;
            retv.is_success=true;
            return retv;
        }
        retBackHomeId(ret.second);
        return retv;
    }

    Home    findHome(int id)
    {
        auto ret=m_id_to_fd.find(id);
        return (*ret).second;
    }

    Id      findId(int cfd)
    {
        auto ret=m_fd_to_id.find(cfd);
        if(ret!=m_fd_to_id.end())
        {
            return ret->second;
        }
        else
        {
            return -1;
        }
    }

    bool    removeFromWaitQue(int cfd)
    {
        auto ret=find(m_waitque.begin(),m_waitque.end(),cfd);
        if(ret!=m_waitque.end())
        {
            m_waitque.erase(ret);
            return true;
        }
        return false;
    }

private:
    vector<int>  m_waitque;
    map<Id,Home> m_id_to_fd;
    map<int,Id>  m_fd_to_id;
    int arry[100]{0};
};

extern Hall hall;

#endif // HALL_H
