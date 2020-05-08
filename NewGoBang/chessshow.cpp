#include "chessshow.h"
#include "ui_chessshow.h"
#include <QMessageBox>
ChessShow::ChessShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChessShow)
{
    ui->setupUi(this);
    this->resize(800,800);
    ui->widget_select->hide();
    ui->widget_NetWar->hide();
    startNetModel();
}

ChessShow::~ChessShow()
{
    delete ui;
}

void ChessShow::mousePressEvent(QMouseEvent *e)
{
    //陷入死循环
    QString info1=QString("(%1,%2)").arg(e->x()).arg(e->y());
    ui->label_show->setText(info1);
    int x_show= around(e->x());
    int y_show= around(e->y());
    QString info2=QString("(%1,%2)").arg(x_show).arg(y_show);
    ui->label_show->setText(info1+info2);

    Node chessman(x_show,y_show);
    if(m_game_type==ChessShow::AiWar)
    {
         aimodel(chessman);

    }
    else if(m_game_type==ChessShow::TwoPeopleWar)
    {
        double_model(chessman);
    }
    else if(m_game_type==ChessShow::NetWar)
    {
        netmodel(chessman);
    }
    update();
}

bool ChessShow::netmodel(Node chessman)
{
    netgame.addChess(chessman.x,chessman.y);
    return true;
}

pair<bool,int> ChessShow::double_model(Node chessman)
{
    auto ret= twgame.addStep(chessman);
    if(ret.second==TwoPeopleGame::FristSuccess)
    {
        update();
        QMessageBox::information(NULL, "fail", "先手赢了",
                                 QMessageBox::Yes |
                                 QMessageBox::No,
                                 QMessageBox::Yes);
        m_game_type=-1;
    }
    else if(ret.second==TwoPeopleGame::SecondSuccess)
    {
        update();
        QMessageBox::information(NULL, "info", "后手赢了",
                                 QMessageBox::Yes |
                                 QMessageBox::No,
                                 QMessageBox::Yes);
        m_game_type=-1;
    }
    return ret;
}

int ChessShow::around(int val)
{
    double db    = ((double)val/perLen);
    double reduce= db-(int)db;
    if(reduce>=0.5)
    {
        return db+1;
    }
    else
    {
        return db;
    }
}

void ChessShow::initImage()
{
    path="./background/1.jpg";
    QImage* img=new QImage;
    if(! ( img->load(path) ) ) //加载图像
    {
        QMessageBox::information(this,
                                 tr("打开图像失败"),
                                 tr("打开图像失败!"));
        delete img;
        return;
    }
    ui->label->setPixmap(QPixmap::fromImage(*img));
    delete img;
}

void ChessShow::paintEvent(QPaintEvent *event)
{
    QPainter p;
    p.begin(this);
    int chessWidth=width()-100;
    int chessHeigth=height()-100;
    p.drawPixmap(0,0,width(),height(),QPixmap("./background/2.jpg"));
    p.drawPixmap(0,0,chessWidth,chessHeigth,QPixmap("./background/1.jpg"));
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    p.setPen(pen);
    perLen=chessWidth/20;

    for(int i=0;i<20;i++)
    {
        p.drawLine(0,perLen*i,chessHeigth,perLen*i);
    }
    for(int i=0;i<20;i++)
    {
        p.drawLine(perLen*i,0,perLen*i,chessHeigth);
    }

    vector<Node> ai_chessman_show;
    vector<Node> play_chessman_show;

    if(m_game_type==ChessShow::AiWar)
    {
        ai_chessman_show=aigame.m_aiact;
        play_chessman_show=aigame.m_playact;
    }
    else if(m_game_type==ChessShow::TwoPeopleWar)
    {
        ai_chessman_show=twgame.m_frist_act;
        play_chessman_show=twgame.m_second_act;
    }
    else if(m_game_type==ChessShow::NetWar)
    {
        ai_chessman_show=netgame.m_player_act;
        play_chessman_show=netgame.m_otherside_act;
    }

    for(auto node:ai_chessman_show)
    {
        p.drawPixmap(node.x*perLen-perLen/2 ,node.y*perLen-perLen/2,
                     perLen,perLen,
                     QPixmap("./chessnode/black.bmp"));

    }

    for(auto node:play_chessman_show)
    {
        p.drawPixmap(node.x*perLen-perLen/2 ,node.y*perLen-perLen/2,
                     perLen,perLen,
                     QPixmap("./chessnode/white.bmp"));
    }

    //标记末尾标签
    if(ai_chessman_show.size()>0)
    {
        Node node=ai_chessman_show.back();
        p.drawPixmap(node.x*perLen-perLen/4 ,node.y*perLen-perLen/4,
                     perLen/2,perLen/2,
                     QPixmap("./chessnode/tag.png"));
    }

    if(play_chessman_show.size()>0)
    {
        Node node=play_chessman_show.back();
        p.drawPixmap(node.x*perLen-perLen/4 ,node.y*perLen-perLen/4,
                     perLen/2,perLen/2,
                     QPixmap("./chessnode/tag.png"));
    }
    p.end();
}

bool ChessShow::aimodel(Node chessman)
{
    auto ret=aigame.playerStep(chessman);
    if(ret.first)
    {
        if(ret.second==AiPlayGame::PlayerSuccess)
        {
            //玩家赢了 打扫战场
            update();
            QMessageBox::information(NULL, "win", "ai失败",
                                     QMessageBox::Yes |
                                     QMessageBox::No,
                                     QMessageBox::Yes);
            m_game_type=-1;
        }
        else
        {
            auto ret=aigame.aiStep();
            update();
            if(ret.second==AiPlayGame::AiSuccess)
            {
                QMessageBox::information(NULL, "fail", "人类一败涂地",
                                         QMessageBox::Yes |
                                         QMessageBox::No,
                                         QMessageBox::Yes);
                m_game_type=-1;
            }
        }
    }
    else
    {
        return false;
    }
    return false;
}

void ChessShow::on_pushButton_ai_first_clicked()
{
    //ai先行
    m_game_type=ChessShow::AiWar;
    aigame.clear();
    aigame.aiStep();
    ui->widget_select->hide();
    update();
}

void ChessShow::on_pushButton_aibehind_clicked()
{
    m_game_type=ChessShow::AiWar;
    aigame.clear();
    ui->widget_select->hide();
    update();
}

void ChessShow::on_pushButton_2_clicked()
{
    ui->widget_select->show();
}

void ChessShow::on_pushButton_clicked()
{
    twgame.clear();
    m_game_type=ChessShow::TwoPeopleWar;

}

void ChessShow::on_pushButton_3_clicked()
{
    m_game_type=ChessShow::NetWar;
    ui->widget_NetWar->show();
}

void ChessShow::on_pushButton_geiveup_clicked()
{
    netgame.giveUpWaiting();
}

void ChessShow::on_pushButton_findteam_clicked()
{
    //查找失败  那就..

    auto lanb=[=](){
    int ret=netgame.findTeam();
    if(ret==NetworkGame::FailFindHome)
    {
        cout<<"寻找失败"<<endl;
        return;
    }
    ui->widget_NetWar->hide();
    if(netgame.getYouAreFrist())
    {
        cout<<"you are frist"<<endl;
        netgame.changeIsMeCanUse();
        return;
    }
    else
    {
        netgame.changeIsMeCanNotUse();
        netgame.theOtherSideStep();
        netgame.changeIsMeCanUse();
        update();
        cout<<"you are second"<<endl;
        return;
    }
    };
    std::thread th(lanb);
    th.detach();
}

void ChessShow::setNetWork(shared_ptr<SockClient> cli)
{
    netgame.setNet(cli);
}

void ChessShow::startNetModel()
{
    std::thread th(&ChessShow::startNetLoop,this);
    th.detach();
}

void ChessShow::startNetLoop()
{
    while (true)
    {
        Node chessman=netgame.getChessFromTube();
        auto ret=netgame.playerStep(chessman);
        update();

        if(ret.first)
        {
            netgame.changeIsMeCanNotUse();
            if(ret.second==NetworkGame::YouselfWin)
            {
                //玩家赢了 打扫战场
                QMessageBox::information(NULL, "success", "you win",
                                         QMessageBox::Yes |
                                         QMessageBox::No,
                                         QMessageBox::Yes);
                m_game_type=-1;
                netgame.clearn();
                netgame.sendSuccess();
            }
            else
            {
                auto ret=netgame.theOtherSideStep();
                netgame.changeIsMeCanUse();
                if(ret.second==NetworkGame::TheOtherSideWin)
                {
                    QMessageBox::information(NULL, "fail", "you fail",
                                             QMessageBox::Yes |
                                             QMessageBox::No,
                                             QMessageBox::Yes);
                    m_game_type=-1;
                    netgame.clearn();
                    netgame.sendSuccess();
                }
            }
            update();
        }
        else
        {
           //点被占用 或者越界
            ui->label_stable->setText("该点不可用");
        }

    }
}

void ChessShow::on_pushButton_5_clicked()
{
    netgame.giveUpWaiting();
}

void ChessShow::on_pushButton_6_clicked()
{
   ui->widget_NetWar->hide();
}

void ChessShow::on_pushButton_7_clicked()
{
    if(m_game_type==TwoPeopleWar)
    {
        twgame.regretGame();
    }
    else if(m_game_type==AiWar)
    {
        aigame.regretGame();
    }
    update();
}

void ChessShow::on_pushButton_8_clicked()
{
    ui->widget_select->hide();
}
