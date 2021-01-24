#include "chessshow.h"
#include "ui_chessshow.h"

#include <QJsonDocument>
#include <QJsonObject>
ChessShow::ChessShow(QWidget *parent) :
    QWidget(parent),
    m_time_out(30),
    m_timestart(false),
    ui(new Ui::ChessShow)
{
    ui->setupUi(this);
    this->resize(800,800);
    ui->widget_select->hide();
    ui->widget_NetWar->hide();

    m_time_id=this->startTimer(1000);
    m_settimeid=true;

    m_write_chess ="./chessnode/white.bmp";
    m_black_chess ="./chessnode/black.bmp";
    m_front_ground="./background/2.jpg";
    m_back_ground ="./background/1.jpg";

    connect(this,&ChessShow::signal_First,     [=](){slot_First();});
    connect(this,&ChessShow::signal_Second,    [=](){slot_Second();});
    connect(this,&ChessShow::signal_GetChess,  [=](QString info){slot_GetChess(info);});
    connect(this,&ChessShow::signal_PerTimeOut,[=](){slot_PerTimeOut();});
    connect(this,&ChessShow::signal_PerLeft,   [=](){slot_PerLeft();});
    connect(this,&ChessShow::signal_PerWon,    [=](){slot_PerWon();});
}


void ChessShow::slot_First()
{
    startTimeOut();
    ui->widget_NetWar->hide();
    m_player_chess=m_write_chess;
    m_other_chess =m_black_chess;
    cout<<"you are frist"<<endl;
    m_game_type=ChessShow::NetWar;
    netgame.setFirstHand();
}

void ChessShow::slot_Second()
{
    ui->widget_NetWar->hide();
    m_player_chess=m_black_chess;
    m_other_chess=m_write_chess;
    m_game_type=ChessShow::NetWar;
    netgame.setSecondHand();
}

void ChessShow::slot_GetChess(QString info)
{
    netgame.getChessFromPer(info);
    update();
    startTimeOut();
}

void ChessShow::slot_PerWon()
{
    QMessageBox::about(nullptr,"perWon","对方赢了");
    netgame.gameOver();
    update();
    stopTimeOut();
}

void ChessShow::slot_PerTimeOut()
{
    QMessageBox::about(nullptr,"pertimeout","对方超时");
    netgame.gameOver();
    update();
    stopTimeOut();
}

void ChessShow::slot_PerLeft()
{
    QMessageBox::about(nullptr,"perleft","你赢了");
    netgame.gameOver();
    update();
    stopTimeOut();
}

ChessShow::~ChessShow()
{
    m_loop_tag=false;
    path.clear();
    perLen=0;
    aigame.~AiPlayGame();
    twgame.clear();
    netgame.gameOver();
    m_loop_tag=false;
    m_time_out=-1;
    m_timestart=false;
    delete ui;
}

void ChessShow::mousePressEvent(QMouseEvent *e)
{
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

void ChessShow::netmodel(Node chessman)
{
    netgame.getChessFromSel(chessman.x,chessman.y);
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
    p.drawPixmap(0,0,width(),height(),QPixmap(m_front_ground));
    p.drawPixmap(0,0,chessWidth,chessHeigth,QPixmap(m_back_ground));
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
                     QPixmap(m_other_chess));

    }

    for(auto node:play_chessman_show)
    {
        p.drawPixmap(node.x*perLen-perLen/2 ,node.y*perLen-perLen/2,
                     perLen,perLen,
                     QPixmap(m_player_chess));
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
    ui->lcdNumber->display(m_time_out);
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
    m_player_chess=m_write_chess;
    m_other_chess=m_black_chess;
    aigame.clear();
    aigame.aiStep();
    ui->widget_select->hide();
    update();
}

void ChessShow::on_pushButton_aibehind_clicked()
{
    m_game_type=ChessShow::AiWar;
    m_player_chess=m_black_chess;
    m_other_chess=m_write_chess;
    aigame.clear();
    ui->widget_select->hide();
    update();
}

void ChessShow::on_pushButton_2_clicked()
{
    ui->widget_select->show();
    aigame.clear();
}

void ChessShow::on_pushButton_clicked()
{
    twgame.clear();
    m_game_type=ChessShow::TwoPeopleWar;

    m_player_chess=m_write_chess;
    m_other_chess=m_black_chess;
}

void ChessShow::on_pushButton_3_clicked()
{
    ui->widget_NetWar->show();
}

void ChessShow::on_pushButton_findteam_clicked()
{
    //查找失败  那就..
    netgame.gameOver();
    netgame.findTeam();
}

void ChessShow::setNetWork(shared_ptr<SockClient> cli)
{
    netgame.setNet(cli);
    m_cli=cli;
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
    //网络对战　悔棋也能添加
    update();
}

void ChessShow::on_pushButton_8_clicked()
{
    ui->widget_select->hide();
}

void ChessShow::closeEvent(QCloseEvent * e)
{
    if( QMessageBox::question(this,
                              tr("Quit"),
                              tr("Are you sure to quit this application?"),
                              QMessageBox::Yes, QMessageBox::No )
            == QMessageBox::Yes)
    {
        e->accept();//不会将事件传递给组件的父组件
        qDebug()<<"ok";
    }
    else
        e->ignore();

}

void ChessShow::on_pushButton_10_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("open a file."),
            "./chessnode",
            tr("images(*.png *jpeg *bmp *jpg);"
               ";video files(*.avi *.mp4 *.wmv);;All files(*.*)"));

    if(fileName.size()==0)
    {
        QMessageBox::information(
                                 NULL,  "error",
                                 "文件选择错误",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes
                    );
        return;
    }

    this->m_black_chess=fileName;
    update();
}

//放弃寻找
void ChessShow::on_pushButton_5_clicked()
{
    QJsonObject json;
    json.insert("type",QString("RefuseFindHome"));
    QJsonDocument document;
    document.setObject(json);
    QByteArray bytearray = document.toJson(QJsonDocument::Compact);
}

//超时
void ChessShow::on_pushButton_TimeOut_clicked()
{
    netgame.gameOver();
    QMessageBox::about(nullptr,"you timeout!!!","您输了");
    netgame.timeOut();
    update();
}

//赢了
void ChessShow::on_pushButton_SelfWon_clicked()
{
    netgame.gameOver();
    QMessageBox::about(nullptr,"you won!!!","您赢了");
    netgame.WonSelf();
    update();
}

void ChessShow::stopTimeOut()
{
    m_time_out=30;
    m_timestart=false;
}

void ChessShow::startTimeOut()
{
    m_time_out=30;
    m_timestart=true;
}

void ChessShow::timerEvent(QTimerEvent *)
{
    if(m_game_type==ChessShow::NetWar&&m_timestart==true)
    {
        if(m_time_out<=0)
        {
            emit ui->pushButton_TimeOut->click();
        }
        else
        {
            m_time_out--;
            cout<<m_time_out<<endl;
        }
        update();
    }
}
