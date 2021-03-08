#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#define Port 8888

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    signal(SIGPIPE,SIG_IGN);

    while (true)
    {
        client =shared_ptr<SockClient>(new SockClient("127.0.0.1",Port));
        if(client->connectToHost()==-1)
        {
            //超时  你可以在这里设置重传次数
        }
        else
        {
            break;
        }
    }
    client->start();
    startReadLoop();
    chess.setNetWork(client);
    chess.show();
}

MainWindow::~MainWindow()
{
    client->~SockClient();
    delete ui;
}

void MainWindow::readLoopDispatch()
{
    while (true)
    {
        int len=0;
        int ret=client->stableRecv((char*)&len,4);
        cout<<"读取到的信息长度"<<len<<endl;
        if(ret>0)
        {
            char *mem=(char*)malloc(len+4);
            memset(mem,0,len+4);

            client->stableRecv(mem,len);
            cout<<"读取到的信息为"<<mem<<" "<<endl;
            QString type=getType(mem);
            cout<<"读取到的信息类型"<<type.toStdString().c_str()<<endl;
            if(type=="First")
            {
                emit chess.signal_First();
            }
            else if(type=="Second")
            {
                emit chess.signal_Second();
            }
            else if(type=="SendChess")
            {
                emit chess.signal_GetChess(QString(mem));
            }
            else if(type=="TimeOut")
            {
                emit chess.signal_PerTimeOut();
            }
            else if(type=="PerWon")
            {
                cout<<"对端赢了"<<mem<<endl;
                emit chess.signal_PerWon();
            }
            else if(type=="PerLeft")
            {
                emit chess.signal_PerLeft();
            }
            else if(type=="TalkToPeer")
            {
                cout<<"gettalking"<<mem<<endl;
                chess.slot_GetTalkToPeer(mem);
            }
            else if(type=="Pong")
            {
                 cout<<"getPong"<<mem<<endl;
            }
            free(mem);
        }
    }
}

QByteArray  MainWindow::getType(QByteArray bytearray)
{
    QJsonParseError jsonError;
    QJsonDocument paserDoc = QJsonDocument::fromJson(bytearray,&jsonError);
    if (jsonError.error == QJsonParseError::NoError)
    {
        QJsonObject paserObj = paserDoc.object();
        if (paserObj.contains("type"))
        {
            return paserObj["type"].toString().toStdString().c_str();
        }
    }
    return "";
}

void   MainWindow::startReadLoop()
{
    std::thread th(&MainWindow::readLoopDispatch,this);
    th.detach();
}

