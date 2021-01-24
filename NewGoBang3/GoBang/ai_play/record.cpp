#include "record.h"

void Record::insertRecord(int x,int y,int val,int tag)
{
    Node node(x,y);
    if(tag==Chessboard::chong)
    {
        if(val==3)
        {
            m_chong_three.push_back(node);
        }
        else if(val==4)
        {
             m_chong_four.push_back(node);
        }
    }
    else if(tag==Chessboard::huo)
    {
        if(val==3)
        {
            m_huo_three.push_back(node);
        }
        else if(val==4)
        {
             m_huo_four.push_back(node);
        }
    }
    if(val==5)
    {
        m_five_ac.push_back(node);
    }
}

void Record::clear()
{
    m_chong_three.clear();
    m_huo_three.clear();
    m_chong_four.clear();
    m_huo_four.clear();
    m_five_ac.clear();
}
