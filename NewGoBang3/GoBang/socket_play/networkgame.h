#ifndef NETWORKGAME_H
#define NETWORKGAME_H
#include "../chessbord/chessboard.h"
#include <set>
#include <vector>
#include <iostream>
#include "../socket/sockclient.h"

using namespace std;
enum NetGameStatus
{
    PointErr,YouSuccess,Ok,NotTurn
};
//这三种状态分别代表　所用的点不合法　　你成功了　　点合法
class NetworkGame
{
public:
    NetworkGame();
    void           gameOver       ();
    void           setNet         (shared_ptr<SockClient> cli);
    void           getChessFromPer(QString info);
    void           getChessFromPer(int x,int y);
    NetGameStatus  getChessFromSel(int x,int y);
    void           setFirstHand   ();                           //设置先手
    void           setSecondHand  ();                           //设置后手
public:
    void           findTeam       ();
    void           sendSuccess    ();
    void           sendChess      (int x,int y);
    void           timeOut        ();
    void           WonSelf        ();
    void           sendTalkingInfo(QString);
public:
    Chessboard     m_chessboard;
    vector<Node>   m_player_act;
    vector<Node>   m_otherside_act;
private:
    shared_ptr<SockClient> client;
    bool                   m_is_set_client=false;
public:
    bool                   m_self_first;          //判断自己是否是先手
    bool                   m_my_turn=false;       //轮到我了
    bool                   m_send_find_home;      //是否发送了寻找房间的信息了
};

#endif // NETWORKGAME_H
