#ifndef TCPLISTEN_H
#define TCPLISTEN_H
#pragma once
#include"head.h"
class TcpListen
{
public:
    TcpListen();
private:
    int lfd;
    sockaddr_in ser;
};

#endif // TCPLISTEN_H
