#ifndef SOCKCLIENT_H
#define SOCKCLIENT_H
#include"../head.h"
#include<mutex>
#include<thread>
#include<signal.h>
#include<sys/signal.h>
#include<QByteArray>
#include<QString>

using namespace std;

struct Pack
{
    int  len; //信息包的长度
    char ch;  //信息的具体内容
};
class SockClient
{
public:
    SockClient();
    ~SockClient();
    SockClient(string ip,int prot);
    int   writeInfo(char *buf,int len);
    void  setIp(string ip);
    void  setProt(int prot);
    void  destory();
    int   connectToHost();
    void  writeQString (QString );
    int   stableRecv   (char *buf,int len);
    int   createSocket (string ip,int prot);
    void  setNoBlock   ();
    void  setBlock     ();
    void  sendPing     ();
private:
    int connectToSer();//非阻塞connect
public:
    void startAlarm();
    void start();
private:
    bool m_starAlarm;
    int cfd;
    sockaddr_in addr;
    mutex mu;
};

#endif // SOCKCLIENT_H
