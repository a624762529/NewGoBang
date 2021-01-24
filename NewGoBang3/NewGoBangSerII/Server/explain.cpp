#include "explain.h"
#include<QDebug>
Explain::Explain()
{
}

void Explain::dispatchServer(QString info,TcpCommunication *com)
{
    QByteArray bytearray=info.toLatin1();
    QJsonParseError jsonError;
    QJsonDocument paserDoc = QJsonDocument::fromJson(bytearray,&jsonError);
    qDebug()<<__LINE__<<info;
    if (jsonError.error == QJsonParseError::NoError)
    {
        QJsonObject paserObj = paserDoc.object();
        if (paserObj.contains("type"))
        {
            QString type=paserObj["type"].toString();
            if(type=="FindHome")
            {
                findHome(info,com);
            }
            else if(type=="SendChess")
            {
                sendChessToPer(info,com);
            }
            else if(type=="Won")
            {
                winSelf(com);
            }
            else if(type=="TimeOut")
            {
                timeOutSelf(com,info);
            }
        }
    }
}

void Explain::findHome(QString info,TcpCommunication *com)
{
    auto ret=hall.addTcpCommunciationToWaitQue(com);
    if(ret==nullptr)
    {
        return;
    }
    else
    {
        //告诉客户端　先手和后手的逻辑
        {
            QJsonObject json;
            json.insert("type",QString("First"));
            QJsonDocument document;
            document.setObject(json);
            QByteArray bytearray = document.toJson(QJsonDocument::Compact);
            ret->first.self->writeQString(bytearray);
            ret->first.status_self=TcpCommunicationStatus::Write;
        }

        {
            QJsonObject json;
            json.insert("type",QString("Second"));
            QJsonDocument document;
            document.setObject(json);
            QByteArray bytearray = document.toJson(QJsonDocument::Compact);
            ret->second.self->writeQString(bytearray);
            ret->first.status_self=TcpCommunicationStatus::Read;
        }
        ret->first.self->changeStatus(TcpCommunication::Status::Write);
        ret->second.self->changeStatus(TcpCommunication::Status::Write);
    }
}

void Explain::sendChessToPer(QString info,TcpCommunication *com)
{
    //将消息给对端发过去
    if(com->getReadArg()==nullptr)
        return;
    qDebug()<<info;
    ClientArg *ret=reinterpret_cast<ClientArg *>(com->getReadArg());
    if(ret->first.self==com)
    {
        qDebug()<<"first write info to per"<<info<<endl;
        if(ret->second.status_self==Read||ret->second.status_self==Write)
        {
            ret->second.self->writeQString(info);
            ret->second.self->changeStatus(TcpCommunication::Status::Write);
        }
        else
        {
            cout<<__FUNCTION__<<__LINE__<<"对端状态非写"<<endl;
            return;
        }
    }
    else if(ret->second.self==com)
    {
        qDebug()<<"second write info to per"<<info<<endl;
        if(ret->first.status_self==Read||ret->first.status_self==Write)
        {
            ret->first.self->writeQString(info);
            ret->first.self->changeStatus(TcpCommunication::Status::Write);
        }
        else
        {
            cout<<__FUNCTION__<<__LINE__<<"对端状态非写"<<endl;
            return;
        }
    }
    //交换双方的状态
    swap(ret->first.status_self,ret->second.status_self);
}

void Explain::winSelf(TcpCommunication *com)
{
    if(com->getReadArg()==nullptr)
        return;
    QJsonObject json;
    json.insert("type",QString("PerWon"));
    QJsonDocument document;
    document.setObject(json);
    QByteArray bytearray = document.toJson(QJsonDocument::Compact);

    ClientArg *ret=reinterpret_cast<ClientArg *>(com->getReadArg());
    TcpCommunication *per=nullptr;
    ArgPacket        *pack=nullptr;
    bool              tag=false;
    if(ret->first.self==com)
    {
        per=ret->second.self;
        pack=&ret->second;
        tag=ret->getSecondCanCommunication();
    }
    else if(ret->second.self==com)
    {
        per=ret->first.self;
        pack=&ret->first;
        tag=ret->getFristCanCommunication();
    }

    if(tag)
    {
        per->writeQString(bytearray);
        per->changeStatus(TcpCommunication::Status::Write);
    }

    //一方释放　则释放掉房间资源
    per->setReadArg(nullptr);
    per->setWriteArg(nullptr);
    com->setReadArg(nullptr);
    com->setWriteArg(nullptr);
    delete ret;
}

void Explain::timeOutSelf(TcpCommunication *com,QString bytearray)
{
    if(com->getReadArg()==nullptr)
        return;

    ClientArg *ret=reinterpret_cast<ClientArg *>(com->getReadArg());
    TcpCommunication *per=nullptr;
    ArgPacket        *pack=nullptr;
    bool              tag=false;
    if(ret->first.self==com)
    {
        per=ret->second.self;
        pack=&ret->second;
        tag=ret->getSecondCanCommunication();
    }
    else if(ret->second.self==com)
    {
        per=ret->first.self;
        pack=&ret->first;
        tag=ret->getFristCanCommunication();
    }

    if(tag)
    {
        per->writeQString(bytearray);
        per->changeStatus(TcpCommunication::Status::Write);
    }

    //一方释放　则释放掉房间资源
    per->setReadArg(nullptr);
    per->setWriteArg(nullptr);
    com->setReadArg(nullptr);
    com->setWriteArg(nullptr);
    delete ret;

}

Hall* Explain::getHall()
{
    return &hall;
}
