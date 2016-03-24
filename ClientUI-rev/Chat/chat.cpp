#include "chat.h"
#include "ui_chat.h"

Chat::Chat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    clnt = new Client();

    error = 0;
    logToFile = false;                   // Default, do not write chat to file

    ui->lineEdit_username->setText("Vivek");
    ui->lineEdit_ip->setText("192.168.0.21");
    ui->lineEdit_port->setText("9654");
}

Chat::~Chat()
{
    delete ui;
}

void Chat::setEditText(const QString &str)
{
    ui->listWidget_messages->addItem(str);
    ui->listWidget_messages->scrollToBottom();
}

void Chat::on_pushButton_chat_clicked()
{
    std::string hostChange;
    username       = ui->lineEdit_username->text();
    hostChange     = ui->lineEdit_ip->text().toStdString();
    port           = ui->lineEdit_port->text().toShort();
    sprintf(host, "%s", hostChange.c_str());
    if( ui->checkBox_logs->isChecked() )
    {
        logToFile = true;
    }
    // std::cerr << logToFile << std::endl;

    if (username.length() > 0 && hostChange.length() > 0 && port > 0)
    {
        std::cerr << host << std::endl;
        if((error = clnt->InitClient(username.toStdString().c_str(), host, port, logToFile)) > 0)
        {
            clnt->CheckError(error);
            ui->label_error->setText("Could not connect!");
        }
        else if( error == 0 )
        {
            ui->label_error->setText("");
            //ui->listWidget_users->addItem(username);
            ui->listWidget_messages->addItem("Connected as " + username);
            clnt->CreateReadThread(this);
            ui->stackedWidget->setCurrentIndex(1);
        }
    }
    else
    {
        ui->label_error->setText("Please enter valid credentials!");
    }

}

void Chat::on_pushButton_sendmsg_clicked()
{
    if(ui->plainTextEdit_msg->toPlainText().toStdString().length() >= MAX_MESSAGE) { return; }
    char user_buf[MAX_BUFFER];
    size_t user_len = strlen(ui->plainTextEdit_msg->toPlainText().toStdString().c_str());
    user_buf[user_len+1] = EOT;
    sprintf(user_buf, "%s", ui->plainTextEdit_msg->toPlainText().toStdString().c_str());
    ui->listWidget_messages->addItem(username + ": " + ui->plainTextEdit_msg->toPlainText());
    ui->listWidget_messages->scrollToBottom();
    ui->plainTextEdit_msg->document()->setPlainText("");

    clnt->SendData(user_buf, user_len+2);

}
