#include "networkgame.h"
#include "../two_people_play/twopeoplegame.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
NetworkGame::NetworkGame()
{
}

void NetworkGame::gameOver()
{
    m_chessboard.clear();
    m_player_act. clear();
    m_otherside_act.clear();

    m_self_first=false;
    m_my_turn=false;
    m_send_find_home=false;
}

void NetworkGame::findTeam()
{
    QJsonObject json;
    json.insert("type",QString("FindHome"));
    QJsonDocument document;
    document.setObject(json);
    QByteArray bytearray = document.toJson(QJsonDocument::Compact);
    client->writeQString(bytearray);
}

void NetworkGame::sendSuccess()
{
    QJsonObject json;
    json.insert("type",QString("Success"));
    QJsonDocument document;
    document.setObject(json);
    QByteArray bytearray = document.toJson(QJsonDocument::Compact);
    client->writeQString(bytearray);
}

void NetworkGame::setNet(shared_ptr<SockClient> cli)
{
    if(m_is_set_client==false)
    {
        client=cli;
        m_is_set_client=true;
    }
    else
    {
        QMessageBox::about(NULL,"title","重复设置　network game socket");
        throw bad_alloc();
    }
}

void NetworkGame::setFirstHand()
{
    m_self_first=true;
    m_my_turn=true;
    m_send_find_home=true;
}

void NetworkGame::setSecondHand()
{
    m_self_first=false;
    m_my_turn=false;
    m_send_find_home=true;
}

NetGameStatus NetworkGame::getChessFromSel(int x,int y)
{
    //自己落子 
    if(m_my_turn==false)
    {
        return NetGameStatus::NotTurn;
    }
    if(!m_chessboard.playChess( x, y,-1))
    {
        return NetGameStatus::PointErr;
    }
    m_chessboard.printfChessboard();
    if(m_chessboard.judeWin(x,y,-1))
    {
        sendSuccess();
        return NetGameStatus::YouSuccess;
    }
    m_my_turn=false;
    sendChess(x,y);
    m_player_act.push_back(Node(x,y));
    return Ok;
}

void NetworkGame::sendChess(int x, int y)
{
    QJsonObject json;
    json.insert("type",QString("SendChess"));
    json.insert("x:",x);
    json.insert("y:",y);
    QJsonDocument document;
    document.setObject(json);
    QByteArray bytearray = document.toJson(QJsonDocument::Compact);
    client->writeQString(bytearray);
}

void NetworkGame::getChessFromPer(int x,int y)
{
    //从对端获取到了　棋子
    if(!m_chessboard.playChess( x, y,1))
    {
        cout<<"点击的点不合法"<<endl;
        return ;
    }
    m_chessboard.printfChessboard();
    m_my_turn=true;
    m_otherside_act.push_back(Node(x,y));
}

void NetworkGame::getChessFromPer(QString info)
{
    QByteArray bytearray=info.toLatin1();
    QJsonParseError jsonError;
    QJsonDocument paserDoc = QJsonDocument::fromJson(bytearray,&jsonError);

    qDebug()<<__LINE__<<"   "<<__FUNCTION__<<info;
    if (jsonError.error == QJsonParseError::NoError)
    {
        QJsonObject paserObj = paserDoc.object();
        getChessFromPer(paserObj["x:"].toInt(),paserObj["y:"].toInt());
    }
}

void NetworkGame::timeOut()
{
    QJsonObject json;
    json.insert("type",QString("TimeOut"));
    QJsonDocument document;
    document.setObject(json);
    QByteArray bytearray = document.toJson(QJsonDocument::Compact);
    client->writeQString(bytearray);
}

void NetworkGame::WonSelf()
{
    QJsonObject json;
    json.insert("type",QString("Won"));
    QJsonDocument document;
    document.setObject(json);
    QByteArray bytearray = document.toJson(QJsonDocument::Compact);
    client->writeQString(bytearray);
}
