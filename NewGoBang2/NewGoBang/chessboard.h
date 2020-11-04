#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include<iostream>
#include<cstdio>
#include<cstring>
#include<iomanip>

using namespace std;

#define MaxX  20
#define MaxY 20
//val = 0 说明该点没有棋子（空位置）
//val =-1 说明该点是白子
//val = 1  说明该点是黑子

class Node
{
public:
    Node()
    {
        this->x=-1;
        this->y=-1;
    }
    Node(int x,int y)
    {
        this->x=x;
        this->y=y;
    }
public:
    int x=0;
    int y=0;
};

class Chessboard
{
public:
    Chessboard();
    ~Chessboard();
    void           printfChessboard();
    pair<int,int>  getHorizontalTagQua(int x,int y,int tag);
    pair<int,int>  getVerticalTagQua  (int x,int y,int tag);
    pair<int,int>  getaPostropheQua   (int x,int y,int tag);
    pair<int,int>  getaPressDownQua   (int x,int y,int tag);
    bool           judeWin(int x,int y,int tag);
    void           cleanPoint(int x,int y);
public:
    bool           playChess(int x,int y,int tag);
    void           clearChess();
private:
    bool           jude_x_ill(int x);
    bool           jude_y_ill(int y);
    int            getStata(bool behind,bool front);
public:
    static bool    judeLegal(Node node);
    static int     getPointStata(int x, int y);
    void           clear();
public:
	static int m_chessboard[MaxX][MaxY];
    enum{huo,chong,si};
};



void operator >>(istream &cin,Node &node);
#endif // CHESSBOARD_H
