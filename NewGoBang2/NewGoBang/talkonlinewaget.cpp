#include "talkonlinewaget.h"
#include "ui_talkonlinewaget.h"

TalkOnlineWaget::TalkOnlineWaget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::TalkOnlineWaget)
{
    ui->setupUi(this);
    initIpProt("127.0.0.1",9999);
}

TalkOnlineWaget::~TalkOnlineWaget()
{
    delete ui;
}

void TalkOnlineWaget::initIpProt(string ip,int prot)
{
    this->m_ip=ip;
    this->m_prot=prot;
    //使用阻塞的模式通讯
    m_client.createSocket(m_ip,m_prot);
    m_client.setBlock();
    m_client.connectToHost();

    std::thread th(&TalkOnlineWaget::readInfo,this);
    th.detach();
}

void TalkOnlineWaget::sendInfo(char *info ,int home_id)
{
    auto ret= createTalkInfo(info,home_id);
    m_client.writeInfo(reinterpret_cast<char*>(ret),ret->info_len+4);

    QString qinfo="你:";
    ui->textEdit_talk_wage->append(qinfo+info);

    ui->textEdit_send_info->clear();
    cleanInfoPack(ret);
}

void TalkOnlineWaget::setHomeId(int id)
{
    m_home_id=id;
}

void TalkOnlineWaget::readInfo()
{
    char buf[4096]{};
    while (true)
    {
        int ret=m_client.readInfo(buf,sizeof(buf));
        if(ret==0)
        {
            m_client.destory();
            break;
        }
        InfoPack *pack=reinterpret_cast<InfoPack *>(buf);
        TalkInfo *info=reinterpret_cast<TalkInfo *>(&pack->m_data);
        QString qinfo="对方:"+QString(&info->m_info);
        ui->textEdit_talk_wage->append(qinfo);
    }

}

void TalkOnlineWaget::on_pushButton_clicked()
{
    QString info=ui->textEdit_send_info->toPlainText();
    sendInfo((char*)info.toStdString().c_str() ,m_home_id);
}

void TalkOnlineWaget::joinHome()
{
    auto ret= createJoinHomeInfo(m_home_id);
    m_client.writeInfo(reinterpret_cast<char*>(ret),ret->info_len+4);
    cleanInfoPack(ret);
}

void TalkOnlineWaget::on_pushButton_2_clicked()
{
    joinHome();
}

void TalkOnlineWaget::startRead()
{

}

void TalkOnlineWaget::sendLeaveHome()
{
    auto ret=createLeaveHomeInfo(m_home_id);
    m_client.writeInfo(reinterpret_cast<char*>(ret),ret->info_len+4);
    cleanInfoPack(ret);
}

void TalkOnlineWaget::on_pushButton_3_clicked()
{
    sendLeaveHome();
}
