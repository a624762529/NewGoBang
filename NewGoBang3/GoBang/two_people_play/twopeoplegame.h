#ifndef TWOPEOPLEGAME_H
#define TWOPEOPLEGAME_H
#include"../chessbord/chessboard.h"
#include<vector>
#include<iostream>
using namespace std;


class TwoPeopleGame
{
public:
    TwoPeopleGame();
    ~TwoPeopleGame();
    pair<bool,int> addStep(Node chessman);
    void clear();
    void regretGame();
private:
    void play();
    pair<bool,int> fristStep(Node chessman);
    pair<bool,int> secondStep(Node chessman);
public:
    Chessboard m_chessboard;
    vector<Node> m_frist_act;
    vector<Node> m_second_act;
    enum{Success=6,FristSuccess=7,SecondSuccess=8};
private:
    bool jude_frist=true;
};

#endif // TWOPEOPLEGAME_H
