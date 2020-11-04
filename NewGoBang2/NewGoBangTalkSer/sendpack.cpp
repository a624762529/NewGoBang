#include "sendpack.h"


sendpack* createInfo(int len,char *info)
{
    sendpack* ret=(sendpack*)malloc(len+4);
    ret->pack_len=len;
    memcpy(&ret->info,info,len);
    return ret;
}
