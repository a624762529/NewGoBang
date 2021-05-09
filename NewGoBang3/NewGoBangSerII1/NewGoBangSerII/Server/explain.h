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
    void  dispatchServer(QString info,TcpCommunication *com); //分发服务的功能
    Hall* getHall();
private:
    void  gameOver(ClientArg *ret,TcpCommunication *per,TcpCommunication *self);
private:
    void  findHome      (QString info,TcpCommunication *com);
    void  sendChessToPer(QString info,TcpCommunication *com);
    void  sendInfoToPer (QString info,TcpCommunication *com);
    void  sendPeerPong  (TcpCommunication *com);
    void  sendPeerRefuseFindHomeSuccess(TcpCommunication *com);
    void  sendPeerRefuseFindHomeFail   (TcpCommunication *com);
    void  sendPeerRequestUndo (TcpCommunication *com);
    void  sendPerrUndoAgree   (TcpCommunication *com,QString info);
    void  sendPerrUndoDisAgree(TcpCommunication *com,QString info);
    void  winSelf             (TcpCommunication *com);
    void  timeOutSelf         (TcpCommunication *com, QString bytearray);

private:
    Hall  hall;
};

#endif // EXPLAIN_H
