#ifndef CLIENTARG_H
#define CLIENTARG_H
#include"./Socket/tcpcommunication.h"

enum TcpCommunicationStatus
{
    //读　　写　自由　下线　　　　超时　　　没有注册socket   成功   在队列里面
    Read,Write,Free,UnderLine,TimeOut,NoLogon,       Sucess,InQueue
};

//read  从服务器读取数据
//write 向服务端写数据
//free  当前的客户状态:不是在网络对战模块

struct ArgPacket
{
    TcpCommunication* self=nullptr;
    int               status_self=NoLogon;
};

class ClientArg
{
public:
    ClientArg();
    void addTcpCommunication(TcpCommunication* my_sock);
    void realse();
    void clientDisconnect(TcpCommunication* com,void* hall);
    bool getFristCanCommunication();
    bool getSecondCanCommunication();
public:
    ArgPacket first;  //先手
    ArgPacket second; //后手
};

#endif // CLIENTARG_H
