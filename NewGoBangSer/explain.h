#ifndef EXPLAIN_H
#define EXPLAIN_H
#include"server.h"
#include"retserver.h"
#include"hall.h"
#include"sockarry.h"
#include<iostream>
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
        ChangeNothing =32
    };
};

#endif // EXPLAIN_H
