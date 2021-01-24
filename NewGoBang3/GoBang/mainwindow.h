#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "chessshow.h"
#include "./socket/sockclient.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit     MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void         readLoopDispatch();
    QString      getType(QString info);
    void         startReadLoop();
private:
    ChessShow               chess;
private:
    Ui::MainWindow         *ui;
    shared_ptr<SockClient>  client;
};

#endif // MAINWINDOW_H
