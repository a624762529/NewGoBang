#ifndef NETWORKGAME_H
#define NETWORKGAME_H
#include"chessboard.h"
#include<QMessageBox>
#include<set>
#include<vector>
#include<iostream>
#include"server.h"
#include"sockclient.h"
#include"retserver.h"
#include"Tube.h"
using namespace std;

class ChessImp:public TubeImple
{
public:
    Node getNode()
    {
        return Node(x,y);
    }
    ChessImp(int x,int y)
    {
        this->x=x;this->y=y;
    }
public:
    int x;
    int y;
};
class NetworkGame
{
public:
    NetworkGame();
    void clear();
    int findTeam();
    int  addStep(Node chessnode);
    void setNet(shared_ptr<SockClient> cli);
    bool getYouAreFrist();
    pair<bool,int> theOtherSideStep();
    void addChess(int x,int y);
    Node getChessFromTube();
    void clearn();
public:
    void play();
    pair<bool,int> playerStep(Node chessman);
    void           sendChess(Node chessman);
    Node           recvChess();
    void           changeIsMeCanUse();
    void           changeIsMeCanNotUse();
    void           sendSuccess();
    void           giveUpWaiting();
public:
    Chessboard     m_chessboard;
    vector<Node>   m_player_act;
    vector<Node>   m_otherside_act;
    enum
    { YouselfWin=7,TheOtherSideWin=8,FailFindHome=9};
private:
    bool                   jude_frist=true;
    bool                   issetclient=false;
    shared_ptr<SockClient> client;
    Tube                   mytube;
    atomic<bool>           m_isme{false};
};

#endif // NETWORKGAME_H
