#ifndef SOCKARRY_H
#define SOCKARRY_H
#include"head.h"
#include"../business/server.h"
#include<iostream>
#include"../business/retserver.h"
using namespace std;

struct SockArry
{
public:
    SockArry(){}

    SockArry(BackHead *back,int send_len)
    {
        m_back    =back;
        m_send_len=send_len;
    }
    void setHomeId(int id)
    {
        this->m_homeid=id;
    }
    int getId()
    {
        return m_homeid;
    }
    void setInfo(BackHead *back,int send_len)
    {
        m_back=back;
        m_send_len=send_len;
    }
    struct recvBuf
    {
    public:
        char      m_getfromfd[10240]{0};
        int       m_getlen=0;
        int       m_alllen=0;
        bool      m_issetlen=false;
    public:
        void addInfo(char *buf,int len,int all_len)
        {
            memcpy(&m_getfromfd[m_getlen],buf,len);
            m_getlen+=len;

        }

        void setAllLen()
        {
            if(m_issetlen==false)
            {
                SendPackImpl *type=reinterpret_cast<SendPackImpl *>(m_getfromfd);
                m_issetlen=true;
                m_alllen=type->m_info_len;
            }
        }

        int getReadLen()
        {
            //讲获取的报文长度返回出去
            return m_getlen;
        }

        bool jude_readOver()
        {
            return m_alllen>=(m_getlen-1);
        }

        void clear()
        {
            memset(m_getfromfd,0,sizeof(m_getfromfd));
            m_getlen=0;
            m_alllen=0;
            m_issetlen=false;
        }

        char* getInfo()
        {
            char *info=(char*)malloc(m_alllen);
            memcpy(info,m_getfromfd,m_getlen);
            return info;
        }
    };
    ~SockArry()
    {
        m_back=nullptr;
        m_send_len=0;
        m_have_sendlen=0;
        memset(&m_buf,0,sizeof(m_buf));
    }
public:
    BackHead *m_back=nullptr;
    int       m_send_len=0;
    int       m_have_sendlen=0;
    recvBuf   m_buf;
    int       m_homeid=-1;
};
#endif // SOCKARRY_H
