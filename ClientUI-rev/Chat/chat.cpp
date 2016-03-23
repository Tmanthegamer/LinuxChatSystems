#include "chat.h"
#include "ui_chat.h"

Chat::Chat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
}

Chat::~Chat()
{
    delete ui;
}

void Chat::on_pushButton_chat_clicked()
{
    username = ui->lineEdit_username->text();
    ui->listWidget_users->addItem(username);
    ui->listWidget_messages->addItem("Connected as " + username);
    ui->stackedWidget->setCurrentIndex(1);

}

void Chat::on_pushButton_sendmsg_clicked()
{
    ui->listWidget_messages->addItem(username + ": " + ui->plainTextEdit_msg->toPlainText());
    ui->plainTextEdit_msg->document()->setPlainText("");
}
