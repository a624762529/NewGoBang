#ifndef RECORD_H
#define RECORD_H

#include<vector>
#include<iostream>
#include"../chessbord/chessboard.h"
using namespace std;

struct Record
{
public:
    void insertRecord(int x,int y,int val,int tag);
    void clear();
public:
    vector<Node> m_chong_three; //冲3
    vector<Node> m_huo_three;   //活3
    vector<Node> m_chong_four;  //活4
    vector<Node> m_huo_four;    //冲4
    vector<Node> m_five_ac;     //死棋

};
#endif // RECORD_H
