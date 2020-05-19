#include "chessboard.h"


int Chessboard::m_chessboard[MaxX][MaxY] = {0};

Chessboard::Chessboard()
{
    memset(m_chessboard,0,sizeof(m_chessboard));
}

//此处的                  x是横坐标是x   y 纵坐标是y====》
//                        arry[y][x]投影下来的效果就是这样的
bool Chessboard::playChess(int x,int y,int tag)
{
    if(tag!=1&&tag!=-1)
    {
        cout<<__LINE__<<"error"<<endl;
        throw bad_alloc();
    }

    if(x>=0&&x<MaxY&&
       y>=0&&y<MaxX&&
            m_chessboard[y][x]==0)
    {
        m_chessboard[y][x]=tag;
        return true;
    }
    return false;
}

void Chessboard::clearChess()
{
     memset(m_chessboard,0,sizeof(m_chessboard));
}

Chessboard::~Chessboard()
{
    clear();
}

void Chessboard::printfChessboard()
{
    cout<<"///////////////////////////////////"<<endl;
    for(int x=-1;x<MaxX;x++)
    {
        cout<<setw(3)<<x;
    }

    cout<<endl;
    for(int x=0;x<MaxX;x++)
    {
        cout<<setw(3)<<x;
        for(int y=0;y<MaxY;y++)
        {
            cout<<setw(3)<<m_chessboard[x][y];
        }
        cout<<endl;
    }
}

void operator >>(istream &cin,Node &node)
{
    cin>>node.x;
    cin>>node.y;
}

bool Chessboard::jude_x_ill(int x)
{
     return !((x>=0)&&(x<MaxY));
}

bool Chessboard::jude_y_ill(int y)
{
     return !((y>=0)&&(y<MaxX));
}

pair<int,int> Chessboard::getHorizontalTagQua(int point_x,int point_y,int tag)
{
    bool behind=false;//沿着这条线 是否被堵住
    bool front =false;
    int ret=-1;
    int y=point_y;
    int x=0;
    for(x=point_x;x<MaxY&&x>=0;x++)
    {
        if(m_chessboard[y][x]==tag)
        {
            ret++;
        }
        else if(m_chessboard[y][x]==(tag*-1) )
        {
            front=true;
            goto Go1;
        }
        else
        {
            break;
        }
    }
    front=jude_x_ill(x);

Go1:

    for(x=point_x;x<MaxY&&x>=0;x--)
    {
        if(m_chessboard[y][x]==tag)
        {
            ret++;
        }
        else if(m_chessboard[y][x]==(tag*-1) )
        {
            behind=true;
            goto Go2;
        }
        else
        {
            break;
        }
    }
  behind=jude_x_ill(x);;
Go2:
    return pair<int,int>(ret,getStata(behind,front));
}

pair<int,int>  Chessboard::getVerticalTagQua(int point_x,int point_y,int tag)
{
    int ret=-1;
    int x=point_x;

    bool behind=false;//沿着这条线 是否被堵住
    bool front =false;

    int y=0;
    for(y=point_y;y<MaxX&&y>=0;y++)
    {
        if(m_chessboard[y][x]==tag)
        {
            ret++;
        }
        else if(m_chessboard[y][x]==(tag*-1))
        {
            front=true;
            goto Go1;
        }
        else
        {
            break;
        }
    }

    front=jude_y_ill(y);
Go1:
    for(y=point_y;y<MaxX&&y>=0;y--)
    {
        if(m_chessboard[y][x]==tag)
        {
            ret++;
        }
        else if(m_chessboard[y][x]==(-1*tag))
        {
            behind=true;
            goto Go2;
        }
        else
        {
            break;
        }
    }
    behind=jude_y_ill(y);
Go2:
    return pair<int,int>(ret,getStata(behind,front));
}

pair<int,int>  Chessboard::getaPostropheQua   (int point_x,int point_y,int tag)
{
    int ret=-1;
    int x=0,y=0;

    bool behind=false;//沿着这条线 是否被堵住
    bool front =false;

    for(y=point_y,x=point_x;
            y<MaxX&&y>=0 && x<MaxX&&x>=0;
            y++,x++)
    {
        if(m_chessboard[y][x]==tag)
        {
            ret++;
        }
        else if(m_chessboard[y][x]==(-1*tag))
        {
            front=true;
            goto Go1;
        }
        else
        {
            break;
        }
    }
    front=jude_x_ill(x);
    front=front||jude_y_ill(y);

Go1:
    for(y=point_y,x=point_x;
            y<MaxX&&y>=0 && x<MaxX&&x>=0;
            y--,x--)
    {
        if(m_chessboard[y][x]==tag)
        {
            ret++;
        }
        else if(m_chessboard[y][x]==(-1*tag))
        {
            behind=true;
            goto Go2;
        }
        else
        {
            break;
        }
    }
    behind=jude_x_ill(x);
    behind=behind||jude_y_ill(y);
Go2:
    return pair<int,int>(ret,getStata(behind,front));
}

pair<int,int>  Chessboard::getaPressDownQua  (int point_x,int point_y,int tag)
{
    int ret=-1;
    int x=0,y=0;

    bool behind=false;//沿着这条线 是否被堵住
    bool front =false;


    for(  y=point_y,x=point_x;
            y<MaxX&&y>=0 && x<MaxY&&x>=0;
            y--,x++)
    {
        if(m_chessboard[y][x]==tag)
        {
            ret++;
        }
        else if(m_chessboard[y][x]==(-1*tag))
        {
            front=true;
            goto Go1;
        }
        else
        {
            break;
        }
    }
    front=jude_x_ill(x);
    front=front||jude_y_ill(y);
    Go1:
    for(  y=point_y,x=point_x;
            y<MaxX&&y>=0 && x<MaxY&&x>=0;
            y++,x--)
    {
        if(m_chessboard[y][x]==tag)
        {
            ret++;
        }
        else if(m_chessboard[y][x]==(-1*tag))
        {
            behind=true;
            goto Go2;
        }
        else
        {
            break;
        }
    }
    behind=jude_x_ill(x);
    behind=behind||jude_y_ill(y);


    Go2:
    return pair<int,int>(ret,getStata(behind,front));
}

int  Chessboard::getStata(bool behind,bool front)
{
    if(behind==false&&front==false)
    {
        return Chessboard::huo;
    }
    else if(behind==true&&front==true)
    {
		return Chessboard::si;
    }
    else if(behind || front)
    {
		return Chessboard::chong;
	}
}

bool Chessboard::judeWin(int x,int y,int tag)
{
    int ret1=  getHorizontalTagQua(x,y,tag).first;
    if(ret1>=5) return true;
    int ret2= getVerticalTagQua   (x,y,tag).first;
    if(ret2>=5) return true;
    int ret3= getaPostropheQua    (x,y,tag).first;
    if(ret3>=5) return true;
    int ret4= getaPressDownQua    (x,y,tag).first;
    if(ret4>=5) return true;
    return false;
}

void  Chessboard::cleanPoint(int x,int y)
{
    m_chessboard[y][x]=0;
}

bool Chessboard::judeLegal(Node node)
{
    return (node.x>=0&&node.x<MaxY)&&(node.y>=0&&node.y<MaxX);
}


int  Chessboard::getPointStata(int x, int y)
{
	return m_chessboard[y][x];
}
 

void Chessboard::clear()
{
    memset(m_chessboard,0,sizeof(m_chessboard));
}
