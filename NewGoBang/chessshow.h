#ifndef CHESSSHOW_H
#define CHESSSHOW_H

#include<QWidget>
#include<QString>
#include<QMouseEvent>
#include<QPainter>
#include<QPaintEvent>
#include<QPen>
#include"networkgame.h"
#include"aiplaygame.h"
#include"twopeoplegame.h"
#include<thread>
#include <QSoundEffect>

#include <QSound>
#include <QMediaPlayer>
using namespace std;
namespace Ui {
class ChessShow;
}

class ChessShow : public QWidget
{
    Q_OBJECT
public:
    explicit ChessShow(QWidget *parent = 0);
    ~ChessShow();
    void            mousePressEvent(QMouseEvent *e);
    void            paintEvent(QPaintEvent *event);
    void            timerEvent(QTimerEvent *);
public:
    void            setNetWork(shared_ptr<SockClient> cli);
private slots:
    void            on_pushButton_ai_first_clicked();
    void            on_pushButton_aibehind_clicked();
    void            on_pushButton_2_clicked();
    void            on_pushButton_clicked();
    void            on_pushButton_3_clicked();
    void            on_pushButton_geiveup_clicked();
    void            on_pushButton_findteam_clicked();
    void            on_pushButton_5_clicked();
    void            on_pushButton_6_clicked();
    void            on_pushButton_7_clicked();
    void            on_pushButton_8_clicked();
private:
    void            initImage();
    int             around(int val);
private:
    bool            aimodel(Node chessman);
    bool            netmodel(Node chessman);
    pair<bool,int>  double_model(Node chessman);
private:
    void            startNetModel();
    void            startNetLoop();
private:
    void            stopTimeOut();
    void            startTimeOut();
    void            dealNetGameOver();
private:
    int             m_game_type=-1;
    Ui::ChessShow  *ui;
    QString         path;
    int             perLen;
    AiPlayGame      aigame;
    TwoPeopleGame   twgame;
    NetworkGame     netgame;
    enum            {TwoPeopleWar,AiWar,NetWar};
    atomic<bool>    m_create_find_team_thread;
    atomic<bool>    m_loop_tag;
    atomic<int>     m_time_out;
    atomic<bool>    m_timestart;

    QString         m_write_chess;
    QString         m_black_chess;
    QString         m_player_chess;
    QString         m_other_chess;
};

#endif // CHESSSHOW_H
