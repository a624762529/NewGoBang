#include "clientarg.h"

#include <QJsonDocument>
#include <QJsonObject>
#include"./Server/hall.h"


ClientArg::ClientArg()
{
    first.status_self=NoLogon;
    second.status_self=NoLogon;
}

void ClientArg::addTcpCommunication(TcpCommunication* my_sock)
{
    //先手没有注册　那就注册先手
    if(first.status_self==TcpCommunicationStatus::NoLogon)
    {
        first.self=my_sock;
        first.status_self=TcpCommunicationStatus::Write;
    }
    else if(second.status_self==TcpCommunicationStatus::NoLogon)
    {
        second.self=my_sock;
        second.status_self=TcpCommunicationStatus::Read;
    }
}

void ClientArg::realse()
{
    //改变文件描述符的状态为可读
    first.self->changeStatus(TcpCommunication::Status::Read);
    second.self->changeStatus(TcpCommunication::Status::Read);
    //设置他们的状态
    second.self->setReadArg(nullptr);
    second.self->setWriteArg(nullptr);
    first.self->setReadArg(nullptr);
    first.self->setWriteArg(nullptr);
}

void ClientArg::clientDisconnect(TcpCommunication* tcpCon, void *hallv)
{
    //一个客户断开链接
    Hall *hall=(Hall *)hallv;
    ClientArg *ret=reinterpret_cast<ClientArg *>(tcpCon->getReadArg());
    if(ret==nullptr)
    {
        hall->removeInQueue(tcpCon);
        return;
    }
    TcpCommunication* per=nullptr;//对端的tcp接口
    ArgPacket       * pack=nullptr;
    if(ret->first.self==tcpCon)
    {
        ret->first.status_self=NoLogon;
        ret->first.self->setReadArg(nullptr);
        ret->first.self->setWriteArg(nullptr);
        per=ret->second.self;
        pack=&ret->second;
    }
    else if(ret->second.self==tcpCon)
    {
        ret->second.status_self=NoLogon;
        ret->second.self->setReadArg(nullptr);
        ret->second.self->setWriteArg(nullptr);
        per=ret->first.self;
        pack=&ret->first;
    }

    //对端处于下线状态
    if(pack->status_self==NoLogon)
    {
        delete ret;
    }
    else
    {
        //给对端发送下线信息
        QJsonObject json;
        json.insert("type",QString("PerLeft"));
        QJsonDocument document;
        document.setObject(json);
        QByteArray bytearray = document.toJson(QJsonDocument::Compact);
        per->appendQString(bytearray);
        per->changeStatus(TcpCommunication::Status::Write);

        per->setReadArg(nullptr);
        per->setWriteArg(nullptr);
        delete ret;
    }

}

bool ClientArg::getFristCanCommunication()
{
    return first.status_self==TcpCommunicationStatus::Read||
            first.status_self==TcpCommunicationStatus::Write;
}

bool ClientArg::getSecondCanCommunication()
{
    return second.status_self==TcpCommunicationStatus::Read||
            second.status_self==TcpCommunicationStatus::Write;
}

void ClientArg::showTwoStatus()
{
    //Read,Write,Free,UnderLine,TimeOut,NoLogon,       Sucess,InQueue
    switch (first.status_self)
    {
    case Read:
        cout<<"先手:read"<<endl;
        break;
    case Write:
        cout<<"先手:write"<<endl;
        break;
    case Free:
        cout<<"先手:free"<<endl;
        break;
    case UnderLine:
        cout<<"先手:underLine"<<endl;
        break;
    case NoLogon:
        cout<<"先手:noLogon"<<endl;
        break;
    case Sucess:
        cout<<"先手:Sucess"<<endl;
        break;
    case InQueue:
        cout<<"先手:InQueue"<<endl;
        break;
    default:
        break;
    }


    //Read,Write,Free,UnderLine,TimeOut,NoLogon,       Sucess,InQueue
    switch (second.status_self)
    {
    case Read:
        cout<<"后手:read"<<endl;
        break;
    case Write:
        cout<<"后手:write"<<endl;
        break;
    case Free:
        cout<<"后手:free"<<endl;
        break;
    case UnderLine:
        cout<<"后手:underLine"<<endl;
        break;
    case NoLogon:
        cout<<"后手:noLogon"<<endl;
        break;
    case Sucess:
        cout<<"后手:Sucess"<<endl;
        break;
    case InQueue:
        cout<<"后手:InQueue"<<endl;
        break;
    default:
        break;
    }
}
