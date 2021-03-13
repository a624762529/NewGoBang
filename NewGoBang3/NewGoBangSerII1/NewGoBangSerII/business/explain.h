#ifndef EXPLAIN_H
#define EXPLAIN_H
#include"server.h"
#include"retserver.h"
#include"hall.h"
#include"../socket/sockarry.h"
#include<iostream>
#include<map>
using namespace std;


class Explain
{
public:
    Explain();
    pair<Home,int> explain(SendPackImpl *back, int cfd,map<int,SockArry> &mp);
    enum
    {
        ChangeAllWrite=1,
        FristWrite    =2,SecondWrite  =4,
        FristRead     =8,SecondRead   =16,
        ChangeNothing =32,ChangeAllReadWrite =108
    };
    enum {Writing,Reading,Freedom};
public:
    void dealPlayerGameOver(int fd);
    void dealPlayerInGameLeft(int fd);
    void dealPlayerLeftWithOutRetBackHome(int fd);
    std::map<int,int> map_fd_to_stata;

};


#endif // EXPLAIN_H
