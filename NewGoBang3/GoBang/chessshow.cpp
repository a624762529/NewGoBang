#include "chessshow.h"
#include "ui_chessshow.h"
#include<QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
ChessShow::ChessShow(QWidget *parent) :
    QWidget(parent),
    m_time_out(60),
    m_timestart(false),
    ui(new Ui::ChessShow)
{
    ui->setupUi(this);
    this->resize(800,800);
    ui->widget_select->hide();
    ui->widget_NetWar->hide();
    m_time_id=this->startTimer(1000);
    qDebug() << "chess构造弹出县城ids" <<QThread::currentThreadId();

    m_settimeid=true;
    m_write_chess ="./chessnode/white.bmp";
    m_black_chess ="./chessnode/black.bmp";
    m_front_ground="./background/2.jpg";
    m_back_ground ="./background/1.jpg";

    connect(this,&ChessShow::signal_First,     this, &ChessShow::slot_First  );
    connect(this,&ChessShow::signal_Second,    this, &ChessShow::slot_Second);
    connect(this,&ChessShow::signal_GetChess,  this, &ChessShow::slot_GetChess);
    connect(this,&ChessShow::signal_PerTimeOut,this, &ChessShow::slot_PerTimeOut);
    connect(this,&ChessShow::signal_PerLeft,   this, &ChessShow::slot_PerLeft);
    connect(this,&ChessShow::signal_PerWon,    this, &ChessShow::slot_PerWon );
    connect(this,&ChessShow::signal_StopGame,  this, &ChessShow::slot_GameOver);

    connect(&m_window,&TalkWindow::sendInfo,   [=](QString info)
    {
        //if(m_game_type==NetWar)
        {
            netgame.sendTalkingInfo(info);
        }
    });


}


void ChessShow::slot_First()
{
    startTimeOut();
    ui->widget_NetWar->hide();
    m_player_chess=m_write_chess;
    m_other_chess =m_black_chess;
    cout<<"you are frist"<<endl;
    m_game_type=ChessShow::NetWar;

    QPixmap myPix("./chessnode/black.bmp");
    ui->label_node->setPixmap(myPix);
    ui->label_node->setScaledContents(true);
    netgame.setFirstHand();
    m_window.show();
}

void ChessShow::slot_Second()
{
    ui->widget_NetWar->hide();
    m_player_chess=m_black_chess;
    m_other_chess=m_write_chess;
    m_game_type=ChessShow::NetWar;

    QPixmap myPix("./chessnode/white.bmp");
    ui->label_node->setPixmap(myPix);
    ui->label_node->setScaledContents(true);
    netgame.setSecondHand();

    m_window.show();
}

void ChessShow::slot_GetChess(QString info)
{
    netgame.getChessFromPer(info);
    update();
    startTimeOut();
}

void ChessShow::slot_GameOver()
{
    static QPixmap myPix("./chessnode/tag.png");
    ui->label_node->setPixmap(myPix);
    ui->label_node->setScaledContents(true);
    m_window.hide();
    QMessageBox::about(nullptr,"gameover", "gameover");
}

void ChessShow::slot_PerWon()
{
    emit signal_StopGame("perWon","对方赢了");
    gameOver();
}

void ChessShow::slot_PerTimeOut()
{
    emit signal_StopGame("pertimeout","对方超时");
    gameOver();
}

void ChessShow::slot_PerLeft()
{
    emit signal_StopGame("perleft","你赢了");
    gameOver();
}

void ChessShow::gameOver()
{
    netgame.gameOver();
    stopTimeOut();
    update();
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
        stopTimeOut();
    }
    update();
}

void ChessShow::netmodel(Node chessman)
{
    switch(netgame.getChessFromSel(chessman.x,chessman.y))
    {
    case NetGameStatus::PointErr:
    {
        ui->label_info->setText(QString("落子不合法"));
        update();
        break;
    }
    case NetGameStatus::YouSuccess:
    {
        ui->label_info->setText(QString("won"));
        emit ui->pushButton_SelfWon->click();
        break;
    }
    case NetGameStatus::Ok:
    {
        ui->label_info->setText(QString("落子成功"));
        break;
    }
    case NetGameStatus::NotTurn:
    {
        ui->label_info->setText(QString("还没轮到你呢，请等待对端"));
        break;
    }
    }
}

pair<bool,int> ChessShow::double_model(Node chessman)
{
    auto ret= twgame.addStep(chessman);
    if(ret.second==TwoPeopleGame::FristSuccess)
    {
        update();
        QMessageBox::about(nullptr,"fail","先手赢了");
        m_game_type=-1;


    }
    else if(ret.second==TwoPeopleGame::SecondSuccess)
    {
        update();
        QMessageBox::about(nullptr,"info","后手赢了");
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
        QMessageBox::information(nullptr,"打开图像失败","打开图像失败");
        delete img;
        return;
    }
    ui->label->setPixmap(QPixmap::fromImage(*img));
    delete img;
}

void ChessShow::paintEvent(QPaintEvent *event)
{
    QPainter               p(this);
    QPen                   pen;

    int chessWidth=width()-100;
    int chessHeigth=height()-100;
    p.drawPixmap(0,0,width(),height(),QPixmap(m_front_ground));
    p.drawPixmap(0,0,chessWidth,chessHeigth,QPixmap(m_back_ground));

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
            QMessageBox::information(nullptr,"win","ai失败");
            m_game_type=-1;
        }
        else
        {
            auto ret=aigame.aiStep();
            update();
            if(ret.second==AiPlayGame::AiSuccess)
            {
                QMessageBox::information(nullptr,"fail","人类一败涂地");
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
        m_window.close();
    }
    else
        e->ignore();

}

void ChessShow::resizeEvent(QResizeEvent *event)
{
    int max_hw=max(event->size().height(),event->size().width());
    resize(max_hw,max_hw);
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
    gameOver();
    //QMessageBox::information(nullptr,"you timeout!!!","您输了");
    emit signal_StopGame("you timeout!!!","您输了");
    netgame.timeOut();
}

//赢了
void ChessShow::on_pushButton_SelfWon_clicked()
{
    gameOver();
    emit signal_StopGame("you won!!!","您赢了");
    netgame.WonSelf();
}

void ChessShow::stopTimeOut()
{
    m_time_out=60;
    m_timestart=false;
}

void ChessShow::startTimeOut()
{
    m_time_out=60;
    m_timestart=true;
}

void ChessShow::timerEvent(QTimerEvent *)
{
    if(m_game_type==ChessShow::NetWar&&m_timestart==true)
    {
        if(m_time_out<=0)
        {
            emit ui->pushButton_TimeOut->click();
            stopTimeOut();
        }
        else
        {
            m_time_out--;
        }
        update();
    }
}

void ChessShow::slot_GetTalkToPeer(QByteArray bytearray)
{
    QJsonParseError jsonError;
    QJsonDocument paserDoc = QJsonDocument::fromJson(bytearray,&jsonError);
    qDebug()<<__LINE__<<"   "<<__FUNCTION__<<paserDoc.object()["info"].
              toString();


    if (jsonError.error == QJsonParseError::NoError)
    {
        QJsonObject paserObj = paserDoc.object();
        m_window.getChessFromPeer(paserObj["info"].toString());
    }
}


void ChessShow::on_pushButton_9_clicked()
{
    m_window.show();
}
