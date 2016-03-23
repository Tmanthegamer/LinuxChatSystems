#include "chat.h"
#include "ui_chat.h"

Chat::Chat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    clnt = new Client();

    error = 0;

    sprintf(host, "%s", "127.0.0.1");
    port = SERVER_TCP_PORT;             // Default port
    logToFile = true;                   // Default, do not write chat to file
}

Chat::~Chat()
{
    delete ui;
}

void Chat::on_pushButton_chat_clicked()
{
    std::string hostChange;
    username       = ui->lineEdit_username->text();
    hostChange     = ui->lineEdit_ip->text().toStdString();
    port           = ui->lineEdit_port->text().toShort();
    sprintf(host, "%s", hostChange.c_str());

    if (username.length() > 0)
    {



        std::cerr << host << std::endl;
        if((error = clnt->InitClient(host, port, logToFile)) > 0)
        {
            clnt->CheckError(error);
            exit(0);
        }
        else if( error == 0 )
        {
            ui->label_error->setText("");
            ui->listWidget_users->addItem(username);
            ui->listWidget_messages->addItem("Connected as " + username);
            ui->stackedWidget->setCurrentIndex(1);
        }

//        if((error = clnt->SendAndReceiveData(username)) > 0)
//        {
//            clnt->CheckError(error);
//            return;
//        }

        ui->stackedWidget->setCurrentIndex(1);

    }
    else
    {
        ui->label_error->setText("Please enter a valid username.");
    }

}

void Chat::on_pushButton_sendmsg_clicked()
{
    ui->listWidget_messages->addItem(username + ": " + ui->plainTextEdit_msg->toPlainText());
    ui->plainTextEdit_msg->document()->setPlainText("");
}
