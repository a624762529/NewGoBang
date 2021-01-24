#ifndef EXPLAIN_H
#define EXPLAIN_H
#include"../Socket/head.h"
#include"../Socket/tcpcommunication.h"
#include"../Server/hall.h"

#include<QString>
#include<QJsonParseError>
#include<QJsonDocument>
#include<QJsonObject>

class Explain
{
public:
    Explain();
public:
    void dispatchServer(QString info,TcpCommunication *com); //分发服务的功能
    void findHome(QString info,TcpCommunication *com);
    void sendChessToPer(QString info,TcpCommunication *com);
    void winSelf(TcpCommunication *com);
    void timeOutSelf(TcpCommunication *com, QString bytearray);
    Hall* getHall();
private:
    Hall hall;
};

#endif // EXPLAIN_H
