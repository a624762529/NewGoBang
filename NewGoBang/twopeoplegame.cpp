#include "twopeoplegame.h"

TwoPeopleGame::TwoPeopleGame()
{
}

void TwoPeopleGame::play()
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

pair<bool,int> TwoPeopleGame::fristStep(Node chessman)
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
        ret.second=TwoPeopleGame::FristSuccess;
    }
    ret.first=true;
    m_frist_act.push_back(chessman);
    return ret;
}

pair<bool,int> TwoPeopleGame::secondStep(Node chessman)
{
    pair<bool,int> ret;
    ret.first=false;
    if(!m_chessboard.playChess(chessman.x,chessman.y,-1))
    {
        return ret;
    }
    m_chessboard.printfChessboard();
    if(m_chessboard.judeWin(chessman.x,chessman.y,-1))
    {
        cout<<"back success"<<endl;
        ret.second=TwoPeopleGame::SecondSuccess;

    }
    ret.first=true;
    m_second_act.push_back(chessman);
    return ret;
}

pair<bool,int> TwoPeopleGame::addStep(Node chessman)
{
    if(jude_frist)
    {
        auto ret=fristStep(chessman);
        if(ret.first)
        {
            jude_frist=false;
        }
        return ret;
    }
    else
    {
        auto ret=secondStep(chessman);
        if(ret.first)
        {
            jude_frist=true;
        }
        return ret;
    }
}

void TwoPeopleGame::clear()
{
    m_chessboard.clear();
    m_frist_act. clear();
    m_second_act.clear();
}

void TwoPeopleGame::regretGame()
{
    if(m_frist_act.size()>=1)
    {
        vector<Node>::iterator it1= m_frist_act.end()-1;
        m_chessboard.cleanPoint(it1->x,it1->y);
        m_frist_act.pop_back();
    }

    if( m_second_act.size()>=1)
    {
        vector<Node>::iterator it2= m_second_act.end()-1;
        m_chessboard.cleanPoint(it2->x,it2->y);
        m_second_act.pop_back();
    }
}
