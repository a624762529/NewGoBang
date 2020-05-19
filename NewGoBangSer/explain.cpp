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
        bool ret_addsuccess=hall.addPeople(cfd);
        if(ret_addsuccess)
        {
            map_fd_to_stata[cfd]=Freedom;
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
                YouFrist *first =new YouFrist;
                YouSecond*second=new YouSecond;
                int rand=random();
                if(rand==0)
                {
                    ret1->second=SockArry(first,0);
                    ret2->second=SockArry(second,0);

                    map_fd_to_stata[home.cfd1]=Writing;
                    map_fd_to_stata[home.cfd2]=Reading;
                }
                else
                {
                    //1是后手
                    ret2->second=SockArry(first,0);
                    ret1->second=SockArry(second,0);

                    map_fd_to_stata[home.cfd1]=Reading;
                    map_fd_to_stata[home.cfd2]=Writing;
                }
                return pair<Home,int>(home,Explain::ChangeAllWrite);
            }

        }
        //房间数目用完了  没有匹配的人
        return pair<Home,int>(home,Explain::ChangeNothing);
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
        int type=0;
        Home ret_home= hall.findHome(homeid);
        int per_fd=ret_home.getPerFd(cfd);
        if(per_fd==-1)
        {
            chess->setPoint(-1,-1);
            auto retv=mp.find(cfd);
            retv->second=SockArry(chess,0);
            ret_home.cfd1=cfd;
            ret_home.cfd2=cfd;
            type=Explain::ChangeAllWrite;

            dealPlayerLeftWithOutRetBackHome(cfd);
            return pair<Home,int>(ret_home,type);
        }


        if(ret_home.cfd1==cfd)
        {
            type=(Explain::FristRead|Explain::SecondWrite);
        }
        else
        {
            type=(Explain::FristWrite|Explain::SecondRead);
        }
        swap(map_fd_to_stata[ret_home.cfd1],map_fd_to_stata[ret_home.cfd2]);


        auto retv=mp.find(per_fd);
        retv->second=SockArry(chess,0);
        return pair<Home,int>(ret_home,type);
        break;
    }
    case SendPackImpl::timeOut:
    {
        SendChessMan *chess     =new SendChessMan;
        //获取当前信息中的节点值  并且作为另一个节点的输入
        chess->setPoint(-1,-1);
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
        map_fd_to_stata[ret_home.cfd1]=Freedom;
        map_fd_to_stata[ret_home.cfd2]=Freedom;
        int per_fd=ret_home.getPerFd(cfd);
        auto retv=mp.find(per_fd);
        retv->second=SockArry(chess,0);

        dealPlayerGameOver(ret_home.cfd1);
        dealPlayerGameOver(ret_home.cfd2);
        return pair<Home,int>(ret_home,type);
        break;
    }
    case SendPackImpl::success:
    {
        Id   id=hall.findId(cfd);
        Home ret_home= hall.findHome(id);
        int type=(FristRead|SecondRead);

        map_fd_to_stata[ret_home.cfd1]=Freedom;
        map_fd_to_stata[ret_home.cfd2]=Freedom;

        dealPlayerGameOver(ret_home.cfd1);
        dealPlayerGameOver(ret_home.cfd2);
        return pair<Home,int>(ret_home,type);
        break;
    }
    case SendPackImpl::giveUpWating:
    {
        hall.removeFromWaitQue(cfd);
        //返回指令
        EndWaitRecv *send=new EndWaitRecv;
        home.cfd1=cfd;
        auto retv=mp.find(cfd);
        retv->second=SockArry(send,0);
        return pair<Home,int>(home,Explain::FristWrite
                              |Explain::SecondRead);
        break;
    }
    default:
        break;
    }
}


void Explain::dealPlayerGameOver(int fd)
{
    //如果fd在等待队列中 那讲其从等待队列上挪开
    hall.removeFromWaitQue(fd);
    //找到文件描述符对应的id
    int id=hall.findId(fd);
    //如果fd在map中有对应的房间号 那就移除之
    hall.removeFromMapFdToId(fd);
    hall.removeFromMapIdToHome(id);
    //如果文件描述符没
    hall.retBackHomeId(id);
    map_fd_to_stata.erase(fd);
}

void Explain::dealPlayerLeftWithOutRetBackHome(int fd)
{
    //如果fd在等待队列中 那讲其从等待队列上挪开
    hall.removeFromWaitQue(fd);
    //找到文件描述符对应的id
    int id=hall.findId(fd);
    //如果fd在map中有对应的房间号 那就移除之
    hall.removeFromMapFdToId(fd);
    hall.removeFromMapIdToHome(id);
    //如果文件描述符没
    map_fd_to_stata.erase(fd);
}

void Explain::dealPlayerInGameLeft(int fd)
{
    //如果fd在等待队列中 那讲其从等待队列上挪开
    hall.removeFromWaitQue(fd);
    //找到文件描述符对应的id
    int id=hall.findId(fd);
    //如果fd在map中有对应的房间号 那就移除之
    hall.removeFromMapFdToId(fd);
    //如果文件描述符没
    hall.retBackHomeId(id);
    map_fd_to_stata.erase(fd);
}
