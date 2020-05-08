#include "explain.h"

Explain::Explain()
{

}

pair<Home,int> Explain::explain(SendPackImpl *back,
                                int cfd,
                                map<int,SockArry> &mp)
{
    Home home;
    switch (back->m_send_type)
    {
    case SendPackImpl::load:
    {
        break;
    }
    case SendPackImpl::login:
    {
        break;
    }
    case SendPackImpl::playWithOther:
    {
        hall.addPeople(cfd);
        auto ret=hall.addHome();
        home=ret.home;
        if(ret.is_success)
        {
            auto ret1=mp.find(home.cfd1);
            if(ret1==mp.end())
            {
                cout<<"用户数据异常"<<endl;
                throw bad_alloc();
            }
            auto ret2=mp.find(home.cfd2);
            if(ret2==mp.end())
            {
                cout<<"用户数据异常"<<endl;
                throw bad_alloc();
            }

            home=ret.home;
            int  type=Explain::ChangeAllWrite;


            YouFrist *first =new YouFrist;
            YouSecond*second=new YouSecond;
            int rand=random();
            if(rand==0)
            {
                ret1->second=SockArry(first,0);
                ret2->second=SockArry(second,0);
            }
            else
            {
                ret2->second=SockArry(first,0);
                ret1->second=SockArry(second,0);
            }
            return pair<Home,int>(home,type);
        }
        else
        {
            //房间数目用完了  没有匹配的人
            return pair<Home,int>(home,Explain::ChangeNothing);
        }
        break;
    }
    case SendPackImpl::sendChess:
    {
        //解释读取的信息
        SendChess    *mychessman=(SendChess*)back;
        SendChessMan *chess     =new SendChessMan;
        //获取当前信息中的节点值  并且作为另一个节点的输入
        chess->setPoint(mychessman->point.x,mychessman->point.y);
        int homeid=hall.findId(cfd);
        Home ret_home= hall.findHome(homeid);
        int type=0;
        if(ret_home.cfd1==cfd)
        {
            type=(Explain::FristRead|Explain::SecondWrite);
        }
        else
        {
            type=(Explain::FristWrite|Explain::SecondRead);
        }

        int per_fd=ret_home.getPerFd(cfd);
        auto retv=mp.find(per_fd);

        retv->second=SockArry(chess,0);
        return pair<Home,int>(ret_home,type);
        break;
    }
    case SendPackImpl::timeOut:
    {
        break;
    }
    case SendPackImpl::success:
    {
        Id   id=hall.findId(cfd);
        Home ret_home= hall.findHome(id);
        hall.retBackHomeId(id);

        hall.removeFdFromRever(ret_home.cfd1);
        hall.removeFdFromRever(ret_home.cfd2);
        int type=(FristRead|SecondRead);
        return pair<Home,int>(ret_home,type);
        break;
    }
    case SendPackImpl::giveUpWating:
    {
        EndWaitRecv *endwaiting=new EndWaitRecv;
        hall.removeFromWaitQue(cfd);
        auto ret=mp.find(cfd);
        ret->second= SockArry(endwaiting,0);
        home.cfd1=cfd;
        return pair<Home,int>(home,Explain::FristWrite|
                                   Explain::SecondRead);
        break;
    }
    default:
        break;
    }
}
