#ifndef TALKONLINEWAGET_H
#define TALKONLINEWAGET_H

#include <QWidget>
#include "head.h"
#include "talkinfo.h"
#include "sockclient.h"
#include "Tube.h"
namespace Ui {
class TalkOnlineWaget;
}

class TalkOnlineWaget : public QWidget
{
    Q_OBJECT
public:
    explicit TalkOnlineWaget(QWidget *parent = 0);
    ~TalkOnlineWaget();
    void initIpProt(string ip,int prot);
    void startRead();
    void sendInfo(char *info,int home_id);
    void readInfo();
    void joinHome();
    void sendLeaveHome();
    void setHomeId(int id);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::TalkOnlineWaget    *ui;
    SockClient              m_client;
    string                  m_ip;
    int                     m_prot;
    int                     m_home_id;
};

#endif // TALKONLINEWAGET_H
