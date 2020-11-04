#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"talkonlinewaget.h"

#include "chessshow.h"
#include "sockclient.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    ChessShow               chess;
    TalkOnlineWaget         talk_waget;

private:
    Ui::MainWindow         *ui;
    shared_ptr<SockClient>  client;
};

#endif // MAINWINDOW_H
