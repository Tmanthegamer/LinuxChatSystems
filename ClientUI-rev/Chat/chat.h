#ifndef CHAT_H
#define CHAT_H

#include <QMainWindow>

namespace Ui {
class Chat;
}

class Chat : public QMainWindow
{
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = 0);
    ~Chat();
    QString username;

private slots:
    void on_pushButton_chat_clicked();

    void on_pushButton_sendmsg_clicked();

private:
    Ui::Chat *ui;
};

#endif // CHAT_H
