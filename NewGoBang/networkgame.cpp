#include "networkgame.h"

#include "twopeoplegame.h"

NetworkGame::NetworkGame():mytube(100)
{
}

void NetworkGame::play()
{
    m_chessboard.printfChessboard();
    while (true)
    {
        Node chessman;
        do
        {
            cin>>chessman;
        }
        while(!m_chessboard.playChess(chessman.x,chessman.y,1));
        m_chessboard.printfChessboard();
        if(m_chessboard.judeWin(chessman.x,chessman.y,1))
        {
            cout<<"back success"<<endl;
            break;
        }

        do
        {
            cin>>chessman;
        }
        while(!m_chessboard.playChess(chessman.x,chessman.y,-1));
        if(m_chessboard.judeWin(chessman.x,chessman.y,-1))
        {
            cout<<"后手胜利"<<endl;
            break;
        }
        m_chessboard.printfChessboard();
    }

}

void NetworkGame::sendChess(Node chessman)
{
    SendChess chess;
    chess.setPoint(chessman.x,chessman.y);
    client->writeInfo(reinterpret_cast<char*>(&chess),sizeof(SendChess));
}

Node NetworkGame::recvChess()
{
    SendChessMan chessper;
    client->stableRecv(reinterpret_cast<char*>(&chessper),sizeof(SendChessMan));
    Node chessman;
    chessman.x=chessper.point.x;
    chessman.y=chessper.point.y;

    return chessman;
}

//先手落子逻辑
pair<bool,int> NetworkGame::playerStep(Node chessman)
{
    pair<bool,int> ret;
    ret.first=false;
    if(!m_chessboard.playChess(chessman.x,chessman.y,1))
    {
        return ret;
    }
    m_chessboard.printfChessboard();
    if(m_chessboard.judeWin(chessman.x,chessman.y,1))
    {
        cout<<"back success"<<endl;
        ret.second=NetworkGame::YouselfWin;
        //发送你胜利的指令让主机判定
        cout<<"you success"<<endl;
        sendChess(chessman);
    }
    else
    {
        sendChess(chessman);
    }
    ret.first=true;
    m_player_act.push_back(chessman);
    return ret;
}

//后手落子逻辑
pair<bool,int> NetworkGame::theOtherSideStep()
{
    //读取对端发来的chess
    Node chessman=recvChess();
    pair<bool,int> ret;
    ret.first=false;
    if(chessman.x==-1&&chessman.y==-1)
    {
        ret.second=NetworkGame::TimeOut;
        return ret;
    }
    if(!m_chessboard.playChess(chessman.x,chessman.y,-1))
    {
        throw bad_alloc();
        return ret;
    }
    m_chessboard.printfChessboard();

    if(m_chessboard.judeWin(chessman.x,chessman.y,-1))
    {
        cout<<"back success"<<endl;
        ret.second=NetworkGame::TheOtherSideWin;
    }
    ret.first=true;
    m_otherside_act.push_back(chessman);
    return ret;
}


void NetworkGame::clear()
{
    m_chessboard.clear();
    m_player_act. clear();
    m_otherside_act.clear();
}


int NetworkGame::findTeam()
{
    if(issetclient)
    {
        PlayWithOther order;
        client->writeInfo(reinterpret_cast<char*>(&order),sizeof(PlayWithOther));
        YouFrist isFrist;
        client->stableRecv(reinterpret_cast<char*>(&isFrist),sizeof(YouFrist));
        if(isFrist.type==YouFrist::youFrist)
        {
            //服务端判定你是先手
            jude_frist=true;
        }
        else if(isFrist.type==YouFrist::youSecond)
        {
            //服务端判定你是后手
            jude_frist=false;
        }
        else if(isFrist.type==YouFrist::endWaiting)
        {
            return FailFindHome;
        }
    }
    return -1;
}

int  NetworkGame::addStep(Node chessman)
{
    auto ret=playerStep(chessman);
    int m_game_type=-1;
    if(ret.first)
    {
        if(ret.second==NetworkGame::YouselfWin)
        {
            //玩家赢了 打扫战场
            QMessageBox::information(NULL, "success", "you win",
                                     QMessageBox::Yes |
                                     QMessageBox::No,
                                     QMessageBox::Yes);
            m_game_type=-1;
        }
        else
        {
            auto ret=theOtherSideStep();
            if(ret.second==NetworkGame::TheOtherSideWin)
            {
                QMessageBox::information(NULL, "fail", "you fail",
                                         QMessageBox::Yes |
                                         QMessageBox::No,
                                         QMessageBox::Yes);
                m_game_type=-1;
            }
        }
    }
    else
    {
        return false;
    }
    return false;
}

void NetworkGame::setNet(shared_ptr<SockClient> cli)
{
    if(!issetclient)
    {
        client=cli;
        issetclient=true;
    }
}

bool NetworkGame::getYouAreFrist()
{
    return jude_frist;
}

void NetworkGame::addChess(int x,int y)
{
    if(m_isme)
        mytube.put(shared_ptr<ChessImp>
                   (new ChessImp(x,y)));
}

Node NetworkGame::getChessFromTube()
{
    shared_ptr<TubeImple>  ret=mytube.get();
    auto gt= reinterpret_cast<ChessImp*>(ret.get());
    return gt->getNode();
}

void NetworkGame::changeIsMeCanUse()
{
    m_isme=true;
}

void NetworkGame::changeIsMeCanNotUse()
{
    m_isme=false;
}

void NetworkGame::clearn()
{
    m_chessboard.clear();
    m_player_act.clear();
    m_otherside_act.clear();
}

void NetworkGame::sendSuccess()
{
    Success success;
    client->writeInfo(reinterpret_cast<char*>(&success),sizeof(Success));
    return;
}

void NetworkGame::giveUpWaiting()
{
    //放弃等待
    GiveUpWating giveup;
    client->writeInfo(reinterpret_cast<char*>(&giveup),sizeof(GiveUpWating));
}

void NetworkGame::sendTimeOut()
{
    //发送超时
    TimeOutSend timeout;
    client->writeInfo(reinterpret_cast<char*>(&timeout),sizeof(TimeOutSend));
}
