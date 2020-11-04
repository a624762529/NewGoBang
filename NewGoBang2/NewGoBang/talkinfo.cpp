#include "talkinfo.h"

InfoPack* createJoinHomeInfo(int home_id)
{
    InfoPack *send_pack=(InfoPack *)
            malloc(sizeof(InfoPack)+sizeof(TalkInfo));
    memset(send_pack,0,sizeof(InfoPack)+sizeof(TalkInfo));
    send_pack->info_len= sizeof(TalkInfo);
    TalkInfo * join_home   =(TalkInfo *)(&send_pack->m_data);
    join_home->m_home_id  =home_id;
    join_home->m_info_len =sizeof(TalkInfo);
    join_home->m_type=TalkInfo::JoinHome;
    join_home->m_info=0;
    return send_pack;
}

InfoPack* createTalkInfo(char *info,int home_id)
{
    InfoPack *send_pack=(InfoPack *)
            malloc(sizeof(InfoPack)+
                   sizeof(TalkInfo)+
                   strlen(info));
    memset(send_pack,0,sizeof(InfoPack)+
           sizeof(TalkInfo)+strlen(info));

    send_pack->info_len= sizeof(TalkInfo)+strlen(info);
    TalkInfo * sendinfo   =(TalkInfo *)(&send_pack->m_data);
    sendinfo->m_home_id  =home_id;
    sendinfo->m_info_len =sizeof(TalkInfo)+strlen(info);
    sendinfo->m_type=TalkInfo::SendInfo;
    sendinfo->m_info=strlen(info);
    memcpy(&sendinfo->m_info,info,strlen(info));

    return send_pack;
}

void cleanInfoPack(InfoPack *&pack)
{
    free(pack);
    pack=nullptr;
}

InfoPack* createLeaveHomeInfo(int home_id)
{
    InfoPack *send_pack=(InfoPack *)
            malloc(sizeof(InfoPack)+sizeof(TalkInfo));
    memset(send_pack,0,sizeof(InfoPack)+sizeof(TalkInfo));
    send_pack->info_len= sizeof(TalkInfo);
    TalkInfo * join_home   =(TalkInfo *)(&send_pack->m_data);
    join_home->m_home_id  =home_id;
    join_home->m_info_len =sizeof(TalkInfo);
    join_home->m_type=TalkInfo::LevelHome;
    join_home->m_info=0;

    return send_pack;
}
