#include "aiplaygame.h"
#pragma once
AiPlayGame::AiPlayGame()
{
    now_me=false;
}

void AiPlayGame::test()
{
	//测试死期的检测
	m_chessboard.playChess(10, 10, AIACT);
	m_chessboard.playChess(10, 9, PLAYACT);
	m_chessboard.playChess(10, 8, PLAYACT);
	m_chessboard.playChess(10, 7, PLAYACT);
	m_chessboard.playChess(10, 5, AIACT);
}

void AiPlayGame::play()
{
    while (true)
    {
        Node chessman;
		chessman = getPoint();
		if (!Chessboard::judeLegal(chessman) && (Chessboard::getPointStata(chessman.x, chessman.y) == 0))
		{
			cout << "err get illegal act" << endl;
			throw bad_alloc();
		}
		
		m_chessboard.playChess(chessman.x, chessman.y, AIACT);
		m_chessboard.printfChessboard();
		m_aiact.push_back(chessman);
		if (m_chessboard.judeWin(chessman.x, chessman.y, AIACT))
		{
			cout << "people 2 success" << endl;
			break;
		}
		
		
		if (m_aiact.size())
		{
			auto lastPoint = *(m_aiact.end()-1);
            cout<< "ai:"  <<  "(" << lastPoint .x
				<<","<<lastPoint.y<< ")" << endl;
		}
        do
        {
            cin>>chessman;
        }
        while(!m_chessboard.playChess(chessman.x,chessman.y,PLAYACT));
        m_chessboard.printfChessboard();

        m_playact.push_back(chessman);
        if(m_chessboard.judeWin(chessman.x,chessman.y,PLAYACT))
        {
            cout<<"people 1 success"<<endl;
            break;
        }  
        m_chessboard.printfChessboard();
    }
}

Node AiPlayGame::findAttDenfpoint()
{
    for(int x=0;x<MaxY;x++)
    {
        for(int y=0;y<MaxY;y++)
        {
            ifPut(x,y);
        }
    }

    //如果攻击点位有活四或者将玩家杀死的棋  那就无视防御直接攻击
    if(m_attack.m_five_ac.size())
    {
        return m_attack.m_five_ac.front();
    }
	if (m_defent.m_five_ac.size())
	{
		return m_defent.m_five_ac.front();
	}

	//如果自己的点位有活四 或者冲四  那就走自己的路
    if(m_attack.m_huo_four.size())
    {
        return m_attack.m_huo_four.front();
    }
    if(m_attack.m_chong_four.size())
    {
        return m_attack.m_chong_four.front();
    }

	if (m_defent.m_huo_four.size())
	{
		return m_defent.m_huo_four.front();
	}
	


	if (m_attack.m_huo_three.size())
	{
		return m_attack.m_huo_three.front();
	}
	if (m_attack.m_chong_three.size())
	{
		return m_attack.m_chong_three.front();
	}
    return Node();
}

void AiPlayGame::ifPut(int x, int y)
{
	{
		//如果该点没有被占用    并且该点合法
		if (Chessboard::getPointStata(x, y) != 0|| !Chessboard::judeLegal(Node(x,y)))
		{
			goto Go1;
		}

		m_chessboard.playChess(x, y, AIACT);
		pair<int, int> ret1 = m_chessboard.getaPostropheQua(x, y, AIACT);
		m_attack.insertRecord(x, y, ret1.first, ret1.second);

		pair<int, int> ret2 = m_chessboard.getaPressDownQua(x, y, AIACT);
		m_attack.insertRecord(x, y, ret2.first, ret2.second);

		pair<int, int> ret3 = m_chessboard.getHorizontalTagQua(x, y, AIACT);
		m_attack.insertRecord(x, y, ret3.first, ret3.second);

		pair<int, int> ret4 = m_chessboard.getVerticalTagQua(x, y, AIACT);
		m_attack.insertRecord(x, y, ret4.first, ret4.second);
		m_chessboard.cleanPoint(x, y);
	 
	}
Go1:
	{
		if (Chessboard::getPointStata(x, y) != 0 || !Chessboard::judeLegal(Node(x, y)))
		{
			goto Go2;
		}
		m_chessboard.playChess(x, y, PLAYACT);
		pair<int, int> ret1 = m_chessboard.getaPostropheQua(x, y, PLAYACT);
		m_defent.insertRecord(x, y, ret1.first, ret1.second);

		pair<int, int> ret2 = m_chessboard.getaPressDownQua(x, y, PLAYACT);
		m_defent.insertRecord(x, y, ret2.first, ret2.second);

		pair<int, int> ret3 = m_chessboard.getHorizontalTagQua(x, y, PLAYACT);
		m_defent.insertRecord(x, y, ret3.first, ret3.second);

		pair<int, int> ret4 = m_chessboard.getVerticalTagQua(x, y, PLAYACT);
		m_defent.insertRecord(x, y, ret4.first, ret4.second);
		m_chessboard.cleanPoint(x, y);
	}
Go2:
	;
}

 

Node AiPlayGame::getPoint()
{
    Node node=findAttDenfpoint();
    if(node.x==-1||node.y==-1)
    {
        if(m_aiact.size())
        {
            //从ai的周围找到一个点位
            node=getAiAround();
        }
        else if(m_playact.size())
        {
            node=getPlayerAround();
        }
		else
		{
			//落子点为中心点
			node.x = MaxX / 2;
			node.y = MaxY / 2;
		}
    }
    m_attack.clear();
    m_defent.clear();
    return node;
}

Node AiPlayGame::getAiAround()
{
    for(Node point:m_aiact)
    {
        Node node5(point.x+1,point.y+1);
        if(Chessboard::judeLegal(node5)&& (Chessboard::getPointStata(node5.x, node5.y) == 0))
        {
            return node5;
        }
        Node node6(point.x-1,point.y-1);
        if(Chessboard::judeLegal(node6)&& (Chessboard::getPointStata(node6.x, node6.y) == 0))
        {
            return node6;
        }
        Node node7(point.x+1,point.y-1);
        if(Chessboard::judeLegal(node7)&&(Chessboard::getPointStata(node7.x, node7.y) == 0))
        {
            return node7;
        }
        Node node8(point.x-1,point.y+1);
        if(Chessboard::judeLegal(node8)&& (Chessboard::getPointStata(node8.x, node8.y) == 0))
        {
            return node8;
        }

		Node node1(point.x + 1, point.y);
		if (Chessboard::judeLegal(node1) && (Chessboard::getPointStata(node1.x, node1.y) == 0))
		{
			return node1;
		}
		Node node2(point.x - 1, point.y);
		if (Chessboard::judeLegal(node2) && (Chessboard::getPointStata(node2.x, node2.y) == 0))
		{
			return node2;
		}
		Node node3(point.x, point.y + 1);
		if (Chessboard::judeLegal(node3) && (Chessboard::getPointStata(node3.x, node3.y) == 0))
		{
			return node3;
		}
		Node node4(point.x, point.y - 1);
		if (Chessboard::judeLegal(node4) && (Chessboard::getPointStata(node4.x, node4.y) == 0))
		{
			return node4;
		}

    }
    return Node(-1,-1);
}

Node AiPlayGame::getPlayerAround()
{
    for(Node point:m_playact)
    {
        Node node1(point.x+1,point.y);
        if(Chessboard::judeLegal(node1))
        {
            return node1;
        }
        Node node2(point.x-1,point.y);
        if(Chessboard::judeLegal(node2))
        {
            return node2;
        }
        Node node3(point.x,  point.y+1);
        if(Chessboard::judeLegal(node3))
        {
            return node3;
        }
        Node node4(point.x,  point.y-1);
        if(Chessboard::judeLegal(node4))
        {
            return node4;
        }

        Node node5(point.x+1,point.y+1);
        if(Chessboard::judeLegal(node5))
        {
            return node5;
        }
        Node node6(point.x-1,point.y-1);
        if(Chessboard::judeLegal(node6))
        {
            return node6;
        }
        Node node7(point.x+1,point.y-1);
        if(Chessboard::judeLegal(node7))
        {
            return node7;
        }
        Node node8(point.x-1,point.y+1);
        if(Chessboard::judeLegal(node8))
        {
            return node8;
        }
    }
    return Node(-1,-1);
}


pair<bool,int> AiPlayGame::aiStep()
{
    pair<bool,int> ret;
    Node chessman;
    chessman = getPoint();
    if (!Chessboard::judeLegal(chessman) && (Chessboard::getPointStata(chessman.x, chessman.y) == 0))
    {
        cout << "err get illegal act" << endl;
        throw bad_alloc();
    }

    m_chessboard.playChess(chessman.x, chessman.y, AIACT);
    m_chessboard.printfChessboard();
    m_aiact.push_back(chessman);
    if (m_chessboard.judeWin(chessman.x, chessman.y, AIACT))
    {
        cout << "people 2 success" << endl;
        ret.second=AiPlayGame::AiSuccess;
    }


    if (m_aiact.size())
    {
        auto lastPoint = *(m_aiact.end()-1);
        cout<< "ai:" <<  "("<< lastPoint .x
            <<","<<lastPoint.y << ")" << endl;
    }
    ret.first=true;
    return ret;
}

pair<bool,int> AiPlayGame::playerStep(Node chessman)
{
    pair<bool,int> ret;
    ret.first=false;
    if(m_chessboard.playChess(chessman.x,chessman.y,PLAYACT))
    {
        m_chessboard.printfChessboard();
        m_playact.push_back(chessman);
        if(m_chessboard.judeWin(chessman.x,chessman.y,PLAYACT))
        {
            cout<<"people 1 success"<<endl;
            ret.second=AiPlayGame::PlayerSuccess;
        }
        m_chessboard.printfChessboard();
        ret.first=true;
    }
    return ret;
}

void AiPlayGame::clear()
{
    m_playact.clear(); //清空 玩家点位
    m_aiact.clear();   //清空 ai的点位
    m_chessboard.clear();//清空棋盘
}

void AiPlayGame::regretGame()
{

    if(m_playact.size()>=1)
    {
        vector<Node>::iterator player=m_playact.end()-1;
        m_chessboard.cleanPoint(player->x,player->y);
        m_playact.pop_back();
    }


    if(m_aiact.size()>=1)
    {
        vector<Node>::iterator ai   =m_aiact.end()-1;
        m_chessboard.cleanPoint(ai->x,ai->y);
        m_aiact.pop_back();
    }
}

AiPlayGame::~AiPlayGame()
{
    m_attack.clear();    //攻击点位
    m_defent.clear();    //防御点位
    m_playact.clear(); //玩家点位
    m_aiact.clear();   //ai的点位
    m_chessboard.clear();
    ai_frist=false;
    now_ai  =false;
    now_me=false;
}
