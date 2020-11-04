#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    signal(SIGPIPE,SIG_IGN);
    while (true)
    {
        client =shared_ptr<SockClient>(new SockClient("127.0.0.1",8888));
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
    chess.setNetWork(client);

    chess.show();
    talk_waget.show();
    chess.initWaget(&talk_waget);
}

MainWindow::~MainWindow()
{
    client->~SockClient();
    delete ui;
    exit(0);
}
