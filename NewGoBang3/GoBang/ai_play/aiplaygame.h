#ifndef AIPLAYGAME_H
#define AIPLAYGAME_H
#include <vector>
#include <iostream>
#include <atomic>

#include "../chessbord/chessboard.h"
#include "record.h"
using namespace std;
#define PLAYACT 1
#define AIACT  -1

class AiPlayGame
{
public:
    AiPlayGame();
    ~AiPlayGame();
    AiPlayGame(const AiPlayGame&)=delete;
    AiPlayGame& operator =(AiPlayGame&)=delete;
    void           play();
    pair<bool,int> aiStep();
    pair<bool,int> playerStep(Node chessman);
    void           clear();
    void           regretGame();
private:
    void           ifPut(int x,int y);
    void           iPlay();
    Node           findAttDenfpoint();
    Node           getPoint();
private:
    Node           getAiAround();
    Node           getPlayerAround();
private:
    void           test();
private:
    Record         m_attack;    //攻击点位
    Record         m_defent;    //防御点位
public:
    vector<Node>   m_playact; //玩家点位
    vector<Node>   m_aiact;   //ai的点位
    Chessboard     m_chessboard;
    enum {Success=6,PlayerSuccess=7,AiSuccess=8};
    bool           ai_frist=true;
    bool           now_ai  =true;
    atomic<bool>   now_me;
};

#endif // AIPLAYGAME_H
