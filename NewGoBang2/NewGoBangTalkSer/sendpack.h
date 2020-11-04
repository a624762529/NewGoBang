#ifndef SENDPACK_H
#define SENDPACK_H
#include"head.h"
#include<iostream>
using namespace std;


struct sendpack
{
    int  pack_len;
    char info;
};


sendpack* createInfo(int len,char *info);
#endif // SENDPACK_H
