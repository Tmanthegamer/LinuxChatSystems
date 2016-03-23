/********************************************************************************
** Form generated from reading UI file 'chat.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHAT_H
#define UI_CHAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Chat
{
public:
    QWidget *centralWidget;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QLineEdit *lineEdit_ip;
    QLabel *label_ip;
    QPushButton *pushButton_chat;
    QLabel *label_username;
    QLabel *label_port;
    QLabel *label_title;
    QLineEdit *lineEdit_port;
    QLineEdit *lineEdit_username;
    QLabel *label_error;
    QWidget *page_2;
    QListWidget *listWidget_users;
    QPlainTextEdit *plainTextEdit_msg;
    QPushButton *pushButton_sendmsg;
    QListWidget *listWidget_messages;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Chat)
    {
        if (Chat->objectName().isEmpty())
            Chat->setObjectName(QStringLiteral("Chat"));
        Chat->resize(573, 370);
        Chat->setStyleSheet(QStringLiteral(""));
        centralWidget = new QWidget(Chat);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setGeometry(QRect(10, -10, 551, 371));
        stackedWidget->setStyleSheet(QStringLiteral("background-color: rgb(170, 163, 163)"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        lineEdit_ip = new QLineEdit(page);
        lineEdit_ip->setObjectName(QStringLiteral("lineEdit_ip"));
        lineEdit_ip->setGeometry(QRect(200, 200, 113, 32));
        lineEdit_ip->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255)"));
        label_ip = new QLabel(page);
        label_ip->setObjectName(QStringLiteral("label_ip"));
        label_ip->setGeometry(QRect(120, 210, 81, 20));
        label_ip->setStyleSheet(QStringLiteral("font: 11pt \"URW Gothic L\";"));
        label_ip->setFrameShape(QFrame::Box);
        label_ip->setFrameShadow(QFrame::Sunken);
        pushButton_chat = new QPushButton(page);
        pushButton_chat->setObjectName(QStringLiteral("pushButton_chat"));
        pushButton_chat->setGeometry(QRect(220, 310, 95, 30));
        pushButton_chat->setStyleSheet(QStringLiteral(""));
        label_username = new QLabel(page);
        label_username->setObjectName(QStringLiteral("label_username"));
        label_username->setGeometry(QRect(120, 150, 81, 20));
        label_username->setStyleSheet(QStringLiteral("font: 11pt \"URW Gothic L\";"));
        label_username->setFrameShape(QFrame::Box);
        label_username->setFrameShadow(QFrame::Sunken);
        label_username->setLineWidth(1);
        label_port = new QLabel(page);
        label_port->setObjectName(QStringLiteral("label_port"));
        label_port->setGeometry(QRect(120, 260, 81, 20));
        label_port->setStyleSheet(QStringLiteral("font: 11pt \"URW Gothic L\";"));
        label_port->setFrameShape(QFrame::Box);
        label_port->setFrameShadow(QFrame::Sunken);
        label_title = new QLabel(page);
        label_title->setObjectName(QStringLiteral("label_title"));
        label_title->setGeometry(QRect(160, 50, 321, 41));
        label_title->setStyleSheet(QStringLiteral("font: 32pt \"VL Gothic\";"));
        lineEdit_port = new QLineEdit(page);
        lineEdit_port->setObjectName(QStringLiteral("lineEdit_port"));
        lineEdit_port->setGeometry(QRect(200, 250, 113, 32));
        lineEdit_port->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255)"));
        lineEdit_username = new QLineEdit(page);
        lineEdit_username->setObjectName(QStringLiteral("lineEdit_username"));
        lineEdit_username->setGeometry(QRect(200, 140, 113, 32));
        lineEdit_username->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255)"));
        lineEdit_username->setClearButtonEnabled(false);
        label_error = new QLabel(page);
        label_error->setObjectName(QStringLiteral("label_error"));
        label_error->setGeometry(QRect(170, 90, 261, 20));
        label_error->setStyleSheet(QStringLiteral(""));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        listWidget_users = new QListWidget(page_2);
        listWidget_users->setObjectName(QStringLiteral("listWidget_users"));
        listWidget_users->setGeometry(QRect(10, 30, 141, 231));
        listWidget_users->setStyleSheet(QStringLiteral("background-color:rgb(255, 255, 255)"));
        plainTextEdit_msg = new QPlainTextEdit(page_2);
        plainTextEdit_msg->setObjectName(QStringLiteral("plainTextEdit_msg"));
        plainTextEdit_msg->setGeometry(QRect(10, 270, 421, 61));
        plainTextEdit_msg->setStyleSheet(QStringLiteral("background-color:rgb(255, 255, 255)"));
        pushButton_sendmsg = new QPushButton(page_2);
        pushButton_sendmsg->setObjectName(QStringLiteral("pushButton_sendmsg"));
        pushButton_sendmsg->setGeometry(QRect(440, 270, 95, 61));
        pushButton_sendmsg->setStyleSheet(QStringLiteral("background-color:rgb(255, 255, 255); text-color: \"black\";"));
        listWidget_messages = new QListWidget(page_2);
        listWidget_messages->setObjectName(QStringLiteral("listWidget_messages"));
        listWidget_messages->setGeometry(QRect(160, 31, 371, 231));
        listWidget_messages->setStyleSheet(QStringLiteral("background-color:rgb(255, 255, 255)"));
        stackedWidget->addWidget(page_2);
        Chat->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Chat);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 573, 26));
        Chat->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Chat);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Chat->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Chat);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Chat->setStatusBar(statusBar);

        retranslateUi(Chat);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Chat);
    } // setupUi

    void retranslateUi(QMainWindow *Chat)
    {
        Chat->setWindowTitle(QApplication::translate("Chat", "Chat", 0));
        lineEdit_ip->setText(QString());
        label_ip->setText(QApplication::translate("Chat", "IP Address", 0));
        pushButton_chat->setText(QApplication::translate("Chat", "Chat!", 0));
        label_username->setText(QApplication::translate("Chat", "Username", 0));
        label_port->setText(QApplication::translate("Chat", "Port", 0));
        label_title->setText(QApplication::translate("Chat", "C H A T", 0));
        lineEdit_port->setText(QString());
        lineEdit_username->setText(QString());
        label_error->setText(QString());
        pushButton_sendmsg->setText(QApplication::translate("Chat", "Send", 0));
    } // retranslateUi

};

namespace Ui {
    class Chat: public Ui_Chat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHAT_H
