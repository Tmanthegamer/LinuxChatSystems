#ifndef CHAT_H
#define CHAT_H

#include <QMainWindow>
#include <sstream>
#include <QDebug>
#include "Client.h"

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
    QString ip;
    Client *clnt;
    int error;
    int userError;
    char host[20];     // Default host
    short port;     // Default port
    bool logToFile; // Default, do not write chat to file

public slots:
    void setEditText(const QString &str);

private slots:
    void on_pushButton_chat_clicked();

    void on_pushButton_sendmsg_clicked();

private:
    Ui::Chat *ui;
};

#endif // CHAT_H
