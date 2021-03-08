#ifndef TALKWINDOW_H
#define TALKWINDOW_H

#include <QWidget>
#include <QString>

namespace Ui {
class TalkWindow;
}

class TalkWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TalkWindow(QWidget *parent = 0);
    ~TalkWindow();
    void getChessFromPeer(QString);
signals:
    void sendInfo(QString info);
private slots:
    void on_pushButton_clicked();

private:
    Ui::TalkWindow *ui;
};

#endif // TALKWINDOW_H
