/*-------------------------------------------------------------------------------------
--  SOURCE FILE:    chat.cpp - Chat Client
--
--  PROGRAM:        Chat.exe
--
--  FUNCTIONS:      Chat::Chat(QWidget *parent)
--                      -- Constructor
--
--                  Chat::~Chat()
--                      -- Destructor
--
--                  void Chat::setEditText(const QString &str)
--
--                  void Chat::on_pushButton_chat_clicked(void)
--
--                  void Chat::on_pushButton_sendmsg_clicked(void)
--
--                  int Connect(void);
--
--
--  DATE:           March 17, 2016
--
--  REVISIONS:      March 23, 2016
--                    Integrated networking to the UI. - Vivek Kalia
--
--  DESIGNERS:      Vivek Kalia
--
--  PROGRAMMERS:    Vivek Kalia
--
--  NOTES:
--  This program entity connects to an operating server where it has the ability to
--  communication with other similar instances of this Client. The Client will be
--  able to see the other clients in the chat room and any messages that is sent
--  within the chat room.
-------------------------------------------------------------------------------------*/

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
            if( error == CANNOTOPENFILE )
            {
                logToFile = false;
                ui->stackedWidget->setCurrentIndex(1);
            }

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

    error = clnt->SendData(user_buf, user_len+2);

    if(error)
    {
        exit(1);
        ui->label_error->setText("Server disconnected!");
        ui->stackedWidget->setCurrentIndex(0);
        while(ui->listWidget_messages->count()>0)
        {
            std::cerr << "hi" << std::endl;
            ui->listWidget_messages->takeItem(0);
        }
    }

}
