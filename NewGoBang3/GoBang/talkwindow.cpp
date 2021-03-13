#include "talkwindow.h"
#include "ui_talkwindow.h"

#include <QJsonDocument>

TalkWindow::TalkWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TalkWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint);
}

TalkWindow::~TalkWindow()
{
    delete ui;
}




void TalkWindow::on_pushButton_clicked()
{
    QString info=ui->textEdit_send_talking_info->toPlainText();


    emit sendInfo(info);
    ui->textEdit_send_talking_info->clear();

    ui->textEdit->append(QString("你:")+info);
    update();
}

void TalkWindow::getChessFromPeer(QString info)
{
    ui->textEdit->append(QString("对方:")+info.toStdString().c_str());
    update();
}
