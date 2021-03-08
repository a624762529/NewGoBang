#ifndef CHESSSHOW_H
#define CHESSSHOW_H
#include <QMessageBox>
#include <QWidget>
#include <QString>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QFileDialog>
#include <QPen>
#include<QDebug>
#include <thread>

#include "talkwindow.h"
#include "./socket_play/networkgame.h"
#include "./ai_play/aiplaygame.h"
#include "./two_people_play/twopeoplegame.h"


using namespace std;
namespace Ui
{
class ChessShow;
}

class ChessShow : public QWidget
{
    Q_OBJECT
public:
    explicit ChessShow(QWidget *parent = 0);
    ~ChessShow();
    void            mousePressEvent(QMouseEvent *e);
    void            paintEvent     (QPaintEvent *event);
    void            timerEvent     (QTimerEvent *);
public:
    void            setNetWork     (shared_ptr<SockClient> cli);
    void            closeEvent     (QCloseEvent * e );
    void            resizeEvent    (QResizeEvent *event);
private slots:
    void            on_pushButton_ai_first_clicked();
    void            on_pushButton_aibehind_clicked();
    void            on_pushButton_2_clicked();
    void            on_pushButton_clicked();
    void            on_pushButton_3_clicked();
    void            on_pushButton_findteam_clicked();
    void            on_pushButton_5_clicked();
    void            on_pushButton_6_clicked();
    void            on_pushButton_7_clicked();
    void            on_pushButton_8_clicked();
    void            on_pushButton_9_clicked();
    void            on_pushButton_TimeOut_clicked();
    void            on_pushButton_SelfWon_clicked();
signals:
    void            signal_First();                       //先手信号
    void            signal_Second();                      //后手信号
    void            signal_GetChess(QString chess_info);  //获取棋子
    void            signal_PerTimeOut();                  //对端超时
    void            signal_PerLeft();                     //对端离开
    void            signal_PerWon();                      //对端赢了
    void            signal_MessageBox(QString title,QString info);
    void            signal_StopGame(QString title,QString info);
public:
    void            slot_First();
    void            slot_Second();
    void            slot_GetChess(QString chess_info);
    void            slot_PerTimeOut();
    void            slot_PerLeft();
    void            slot_PerWon();
    void            slot_GameOver();
    void            slot_GetTalkToPeer(QByteArray info);
private:
    void            gameOver();
    void            initImage    ();
    int             around       (int val);
private:
    bool            aimodel     (Node chessman);
    void            netmodel    (Node chessman);
    pair<bool,int>  double_model(Node chessman);

    void            stopTimeOut  ();
    void            startTimeOut ();
private:
    int                    m_game_type=-1;
    Ui::ChessShow         *ui;
    QString                path;
    int                    perLen;
    AiPlayGame             aigame;
    TwoPeopleGame          twgame;
    NetworkGame            netgame;
    TalkWindow             m_window;
    enum                   {TwoPeopleWar,AiWar,NetWar};
    atomic<bool>           m_loop_tag;
    atomic<int>            m_time_out;    //超时时间　数字表的显示计数
    atomic<bool>           m_timestart;
    QString                m_write_chess;
    QString                m_black_chess;
    QString                m_player_chess;
    QString                m_other_chess;
    QString                m_front_ground;
    QString                m_back_ground ;
    int                    m_time_id;
    bool                   m_settimeid;
    shared_ptr<SockClient> m_cli;

};

#endif // CHESSSHOW_H
