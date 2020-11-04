#ifndef TALKINFO_H
#define TALKINFO_H
#include"head.h"
using namespace std;
struct InfoPack
{
    int  info_len;
    char m_data;
};
struct TalkInfo
{
    enum
    {
        JoinHome,SendInfo,SendSuccess,SendFail,LevelHome
    };
    int  m_info_len; //信息的长度
    int  m_home_id;  //信息的id
    int  m_type;
    char m_info;     //信息的内容
};

InfoPack* createJoinHomeInfo(int home_id);          //创建一个joinhome的数据包
InfoPack* createTalkInfo(char *info,int home_id);   //创建一个发送的信息
InfoPack* createLeaveHomeInfo(int home_id);
void cleanInfoPack(InfoPack *&pack);                //清理信息
#endif // TALKINFO_H
