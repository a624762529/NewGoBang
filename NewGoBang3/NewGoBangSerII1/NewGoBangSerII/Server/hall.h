#ifndef HALL_H
#define HALL_H
#include<iostream>
#include<vector>
#include<map>
#include"../Socket/tcpcommunication.h"
#include"../clientarg.h"

using namespace std;

class Hall
{
public:
    Hall();
    ClientArg* addTcpCommunciationToWaitQue(TcpCommunication *con);
    int        getHomeId();
    int        rollBackId(int id);
    bool       removeInQueue(TcpCommunication *con);
private:
    char                          homeTag[1024];  //房间id
    vector<TcpCommunication *>    vec;            //等待队列
};

#endif // HALL_H
