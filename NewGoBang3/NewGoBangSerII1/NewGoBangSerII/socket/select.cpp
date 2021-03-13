#include "select.h"

int Select::createSock(int port)
{
    struct sockaddr_in serv_addr;
    socklen_t serv_len = sizeof(serv_addr);
    // 创建套接字
    lfd = socket(AF_INET, SOCK_STREAM, 0);
    // 初始化服务器 sockaddr_in
    memset(&serv_addr, 0, serv_len);
    serv_addr.sin_family = AF_INET;                   // 地址族
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // 监听本机所有的IP
    serv_addr.sin_port = htons(port);                 // 设置端口
    // 绑定IP和端口
    bind(lfd, (struct sockaddr*)&serv_addr, serv_len);
    // 设置同时监听的最大个数
    listen(lfd, 1024);
    printf("Start accept ......\n");
    FD_SET(lfd, &m_read);
    addr=serv_addr;
    return lfd;
}


void Select::startSpy()
{
    thread th(&Select::spy,this);
    th.detach();
}

void Select::spy()
{
    signal(SIGPIPE,SIG_IGN);
    while (true)
    {
       // cout<<"并发连接数目:"<<mp.size()<<" :"<<mp_heart.size() <<endl;
        sleep(4);
    }
}

void Select::loop()
{
    signal(SIGPIPE,SIG_IGN);
    //startSpy();
    int maxfd = lfd;
    while(1)
    {
        // 委托内核做IO检测
        fd_set t_read=m_read;      //读集
        fd_set t_write=m_write;    //写集
        fd_set t_exp=m_exp;        //异常集合

        int ret = select(maxfd+1, &t_read, &t_write, &t_exp, NULL);
        if(ret == -1)
        {
            perror("select error");
            //exit(1);
        }
        // 客户端发起了新的连接
        if(FD_ISSET(lfd, &t_read))
        {
            int cfd=acceptCallBack();
            if(cfd==-1)
            {
                perror("accept err::");
            }
            else
            {
                maxfd = maxfd < cfd ? cfd : maxfd;
            }
        }

        if(FD_ISSET(m_alarm.getReadFd(), &t_read))
        {
            //心跳
            int al_fd=m_alarm.getReadFd();
            char buf[1024];
            int ret=read(al_fd,buf,1024);
            doAlarm();
        }

        // 已经连接的客户端有数据到达
        for(int i=lfd+1; i<=maxfd; ++i)
        {
            if(FD_ISSET(i, &t_read))
            {
                readCallBack(i);
            }
            else if(FD_ISSET(i, &t_write))
            {
                 writeCallBack(i);
            }
            else if(FD_ISSET(i, &t_exp))
            {
                 //cout<<"heart coming"<<endl;
                 expCallBack(i);
            }
        }
    }
}

int Select::acceptCallBack()
{
    // 接受连接请求 - accept不阻塞
    struct sockaddr_in client_addr;
    socklen_t cli_len = sizeof(client_addr);
    int cfd = accept(lfd, (struct sockaddr*)&client_addr, &cli_len);
    if(cfd == -1)
    {
        perror("accept error");
    }

    char ip[64];
    printf("new client IP: %s, Port: %d\n",
           inet_ntop(AF_INET, &client_addr.sin_addr.s_addr,
                     ip, sizeof(ip)),
           ntohs(client_addr.sin_port));
    // 将cfd加入到待检测的读集合中 - 下一次就可以检测到了
    FD_SET(cfd, &m_read);
    FD_SET(cfd, &m_exp);

    mp.insert(pair<int,SockArry>(cfd,SockArry()));
    auto i=HeartNode(cfd);
    mp_heart.insert(pair<int,HeartNode>(cfd,i));
    m_alarm.PushNode(i);
    // 更新最大的文件描述符
    return cfd;
}

void Select::writeCallBack(int cfd)
{

    map<int,SockArry>::iterator it=mp.find(cfd);
    if(it==mp.end())
    {
        perror("error map中没有对应的内容");
        return;
    }
    else
    {
        doWrite(it->second.m_back,cfd);
    }
}

void Select::expCallBack(int i)
{
    char ch;
    int n = recv(i, &ch,1, MSG_OOB) ;
    if(n>0)
    {
        auto ret=mp_heart.find(i);
        (*ret).second.Add();
    }
}

void Select::readCallBack(int cfd)
{
    char buf[10240] = {0};
    int len = recv(cfd, buf, sizeof(buf), 0);
    if(len == -1)
    {
        perror("recv error");
        //exit(1);
    }
    else if(len == 0)
    {
        cout<<"client close connect_:"<<cfd<<endl;
        dealPlayerStopGame(cfd);
        removeFd(cfd);
    }
    else
    {
        SendPackImpl *type=reinterpret_cast<SendPackImpl *>(buf);
        int getAllLen=type->m_info_len;
        map<int,SockArry>::iterator it=mp.find(cfd);
        if(it==mp.end())
        {
            perror("error map中没有对应的内容");
            return;
        }
        it->second.m_buf.addInfo(buf,len,getAllLen);
        if(it->second.m_buf.getReadLen()>=sizeof(SendPackImpl))
        {
            it->second.m_buf.setAllLen();
        }
        else
        {
            return;
        }
        if(it->second.m_buf.jude_readOver())
        {
            char *get_ready_read=
                    it->second.m_buf.getInfo();
            dealExplain(get_ready_read,cfd);
            it->second.m_buf.clear();
            free(get_ready_read);
        }
    }
}

void Select::changeFdWrite(int cfd)
{
    //FD_CLR(cfd, &m_read );//将文件描述法从读集中移除
    FD_SET(cfd, &m_write);//讲文件免赔舒服加入到写集
}

void Select::changeFdRead(int cfd)
{
    FD_CLR(cfd, &m_write );//将文件描述法从写集中移除
    FD_SET(cfd, &m_read);  //讲文件描述符加入到读集
}

void Select::changeFdRdWr(int cfd)
{
    FD_SET(cfd, &m_write);
    FD_SET(cfd, &m_read);
}

void Select::removeFd(int cfd)
{
    close(cfd);
    // 从读集合中删除
    FD_CLR(cfd, &m_read);
    FD_CLR(cfd, &m_write);
    FD_CLR(cfd, &m_exp);

    auto it_find=mp.find(cfd);
    if(it_find!=mp.end())
    {
        mp.erase(cfd);
    }

    auto ret=mp_heart.find(cfd);
    if(ret!=mp_heart.end())
    {
        (*ret).second.destory();
        mp_heart.erase(cfd);
    }
}

void Select::doAlarm()
{
    vector<int> vec=m_alarm.move();
    for(auto i:vec)
    {
        removeFd(i);
    }
}

Select::Select(int port)
{
    FD_ZERO(&m_read);
    FD_ZERO(&m_write);
    FD_ZERO(&m_exp);
    this->createSock(port);
}

Select::~Select()
{
    close(lfd);
}


void Select::doWrite(BackHead *send_back,int cfd)
{
    if(send_back==nullptr)
    {
        return;
    }
    int len=send_back->m_len;          //获取sendback的总长度
    auto it=mp.find(cfd);              //寻找cfd对应的信息包
    if(it==mp.end())
    {
        return;
    }
    int  have_write=it->second.m_send_len;
    char *send_info=reinterpret_cast<char*>(send_back)+have_write;
    int ret=write(cfd,send_info,len-have_write);  //将sendback写入到cfd对应的文件描述符中
    if(ret>0)
    {
        it->second.m_send_len+=ret;
        //信息的总长度等于 已经发送的信息的总长度
        if(it->second.m_send_len==it->second.m_back->m_len)
        {
            free(send_back);                   //删除send
            changeFdRead(cfd);
        }
    }
}

void Select::dealExplain(char *info,int cfd)
{
    if(info==nullptr)
    {
        return;
    }

    auto ret=m_explain.explain(reinterpret_cast<SendPackImpl *>
                               (info),cfd,mp);
    Home home=ret.first;
    if(ret.second==Explain::ChangeAllWrite)
    {
        if(home.cfd1!=-1)
            changeFdWrite(home.cfd1);
        if(home.cfd2!=-1)
            changeFdWrite(home.cfd2);
    }
    else if(ret.second==(Explain::FristRead|Explain::SecondWrite))
    {
        if(home.cfd1!=-1)
            changeFdRead (home.cfd1);
        if(home.cfd2!=-1)
            changeFdWrite(home.cfd2);
    }
    else if(ret.second==(Explain::FristWrite|Explain::SecondRead))
    {
        if(home.cfd1!=-1)
            changeFdWrite(home.cfd1);
        if(home.cfd2!=-1)
            changeFdRead (home.cfd2);
    }
    else if(ret.second==(Explain::FristRead|Explain::SecondRead))
    {
        if(home.cfd1!=-1)
            changeFdRead (home.cfd1);
        if(home.cfd2!=-1)
            changeFdRead (home.cfd2);
    }
    else if(ret.second== Explain::ChangeAllReadWrite)
    {
        //让home里面所有的文件描述符都可读+ 可写
        changeFdRdWr(home.cfd1);
        changeFdRdWr(home.cfd2);
    }
    else if(ret.second==Explain::ChangeNothing)
    {
        return;
    }
}

void Select::dealPlayerStopGame(int cfd)
{
    SendChessMan *chess     =new SendChessMan;

    //获取当前信息中的节点值  并且作为另一个节点的输入
    chess->setPoint(-1,-1);
    //玩家没有在网络游戏中就返回
    int homeid=hall.findId(cfd);
    if(homeid!=-1)
    {
        Home &ret_home= hall.findHome(homeid);
        if(ret_home.getPerFd(cfd)==-1)
        {
            m_explain.dealPlayerGameOver(cfd);
            delete chess;
            return;
        }
        if(m_explain.map_fd_to_stata[ret_home.getPerFd(cfd)]
                ==Explain::Writing)
        {
             if(ret_home.cfd1==cfd)
             {
                 ret_home.cfd1=-1;
             }
             else if(ret_home.cfd2==cfd)
             {
                 ret_home.cfd2=-1;
             }
             m_explain.dealPlayerInGameLeft(cfd);
             delete chess;
             return;
        }
        if(ret_home.cfd1==cfd)
        {
            changeFdWrite(ret_home.cfd2);
        }
        else
        {
            changeFdWrite(ret_home.cfd1);
        }
        int per_fd=ret_home.getPerFd(cfd);
        auto retv=mp.find(per_fd);
        retv->second=SockArry(chess,0);
        m_explain.dealPlayerGameOver(ret_home.cfd1);
        m_explain.dealPlayerGameOver(ret_home.cfd2);
    }
    return ;
}

void Select::startAlarm()
{
    m_alarm.setAlarm(PERALARM);
    int fd_read=m_alarm.getReadFd();
    FD_SET(fd_read, &m_read);
    m_alarm.startAlarm();
}
