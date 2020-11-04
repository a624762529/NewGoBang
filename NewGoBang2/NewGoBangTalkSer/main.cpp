#include"epollloop.h"
#include<iostream>
#include<set>
#include"talkhome.h"
#include"sendpack.h"
using namespace std;

ChatHome MyHome;
void readCallBack(char* arry,int len,void *arg,TcpCommunication *tcpCon)
{
    if(len>0)
    {
        EpollLoop *loop=(EpollLoop *)arg;
        TalkInfo *info=reinterpret_cast<TalkInfo*>(arry);
        switch (info->m_type) {
        case TalkInfo::JoinHome:
        {
            int id=info->m_home_id;
            //讲id加入进去
            MyHome.addHomeId(id);
            MyHome.addTcpCommunic(id,tcpCon);
            break;
        }
        case TalkInfo::SendInfo:
        {
            int id=info->m_home_id;
            Home homev= MyHome.findIdHome(id);
            homev.addTcpCommunication(tcpCon);
            TcpCommunication * percon=homev.getPerCommunic(tcpCon);
            if(percon==nullptr)
            {
                cout<<"对端不存在"<<endl;
                break;
            }
            percon->setWriteInfoLen(len);
            percon->addInfoToWrite(arry,len);
            loop->changeTcpComunicationType(percon,EPOLLOUT);
            percon->changeStatus(TcpCommunication::Status::Write);
            break;
        }
        case TalkInfo::LevelHome:
        {
            MyHome.tcpLevel(tcpCon);
        }
        default:
            break;
        }


    }
    else if(len==0)
    {
        //cout<<"断开链接"<<__FUNCTION__<<__LINE__<<endl;
        int id= MyHome.getId(tcpCon);
        if(MyHome.tcpLevel(tcpCon)!=-1)
        {
            //执行 相关的清理工作
            if(MyHome.findIdHome(id).judeHomeEmpty())
            {
                MyHome.cleanHomeId(id);
            }
        }
    }
}

void writeCallBack(void *arg,TcpCommunication *tcpCon)
{
    //cout<<"写入的信息长度:"<<len<<endl;
    tcpCon->changeStatus(TcpCommunication::Status::Read);
}

void acceptCallBack(TcpCommunication *tcpCon,EpollLoop *loop)
{
    cout<<"accept callback"<<endl;
    tcpCon->setReadCallBack(readCallBack,loop);
    tcpCon->setWriteCallBack(writeCallBack,loop);
}


int main()
{
    EpollLoop loop(9999);
    loop.setCanUse();
//    loop.initHeartTime(2);
//    loop.startAlarm();
    loop.setAcceptcb(acceptCallBack);

    loop.loop();
    return 1;
}
