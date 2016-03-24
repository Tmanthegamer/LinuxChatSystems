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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
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
    QCheckBox *checkBox_logs;
    QLabel *label_tag;
    QFrame *line;
    QWidget *page_2;
    QPlainTextEdit *plainTextEdit_msg;
    QPushButton *pushButton_sendmsg;
    QListWidget *listWidget_messages;
    QLabel *label;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *Chat)
    {
        if (Chat->objectName().isEmpty())
            Chat->setObjectName(QStringLiteral("Chat"));
        Chat->resize(484, 391);
        Chat->setStyleSheet(QStringLiteral(""));
        centralWidget = new QWidget(Chat);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setGeometry(QRect(10, -10, 551, 371));
        stackedWidget->setStyleSheet(QStringLiteral(""));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        lineEdit_ip = new QLineEdit(page);
        lineEdit_ip->setObjectName(QStringLiteral("lineEdit_ip"));
        lineEdit_ip->setGeometry(QRect(110, 240, 113, 32));
        lineEdit_ip->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255)"));
        label_ip = new QLabel(page);
        label_ip->setObjectName(QStringLiteral("label_ip"));
        label_ip->setGeometry(QRect(110, 220, 111, 20));
        label_ip->setStyleSheet(QStringLiteral("font: 11pt \"URW Gothic L\"; border-radius: 10px;"));
        label_ip->setFrameShape(QFrame::Box);
        label_ip->setFrameShadow(QFrame::Sunken);
        label_ip->setAlignment(Qt::AlignCenter);
        pushButton_chat = new QPushButton(page);
        pushButton_chat->setObjectName(QStringLiteral("pushButton_chat"));
        pushButton_chat->setGeometry(QRect(230, 280, 111, 51));
        QFont font;
        font.setFamily(QStringLiteral("Android Emoji"));
        font.setBold(true);
        font.setWeight(75);
        pushButton_chat->setFont(font);
        pushButton_chat->setStyleSheet(QStringLiteral("background-color: rgb(158, 204, 143); text-color: \"white\"; border-radius: 15px; color: #333333;"));
        pushButton_chat->setFlat(false);
        label_username = new QLabel(page);
        label_username->setObjectName(QStringLiteral("label_username"));
        label_username->setGeometry(QRect(110, 280, 111, 20));
        label_username->setStyleSheet(QStringLiteral("font: 11pt \"URW Gothic L\"; border-radius: 5px;"));
        label_username->setFrameShape(QFrame::Box);
        label_username->setFrameShadow(QFrame::Sunken);
        label_username->setLineWidth(1);
        label_username->setAlignment(Qt::AlignCenter);
        label_port = new QLabel(page);
        label_port->setObjectName(QStringLiteral("label_port"));
        label_port->setGeometry(QRect(230, 220, 111, 20));
        label_port->setStyleSheet(QStringLiteral("font: 11pt \"URW Gothic L\"; border-radius: 5px;"));
        label_port->setFrameShape(QFrame::Box);
        label_port->setFrameShadow(QFrame::Sunken);
        label_port->setAlignment(Qt::AlignCenter);
        label_title = new QLabel(page);
        label_title->setObjectName(QStringLiteral("label_title"));
        label_title->setGeometry(QRect(20, 30, 421, 121));
        label_title->setStyleSheet(QStringLiteral("font: 32pt \"VL Gothic\"; "));
        label_title->setFrameShape(QFrame::Box);
        label_title->setFrameShadow(QFrame::Sunken);
        label_title->setAlignment(Qt::AlignCenter);
        lineEdit_port = new QLineEdit(page);
        lineEdit_port->setObjectName(QStringLiteral("lineEdit_port"));
        lineEdit_port->setGeometry(QRect(230, 240, 113, 32));
        lineEdit_port->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255)"));
        lineEdit_username = new QLineEdit(page);
        lineEdit_username->setObjectName(QStringLiteral("lineEdit_username"));
        lineEdit_username->setGeometry(QRect(110, 300, 113, 32));
        lineEdit_username->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255)"));
        lineEdit_username->setClearButtonEnabled(false);
        label_error = new QLabel(page);
        label_error->setObjectName(QStringLiteral("label_error"));
        label_error->setGeometry(QRect(90, 160, 271, 20));
        label_error->setStyleSheet(QStringLiteral("text-color: rgb(255, 0, 0);"));
        label_error->setAlignment(Qt::AlignCenter);
        checkBox_logs = new QCheckBox(page);
        checkBox_logs->setObjectName(QStringLiteral("checkBox_logs"));
        checkBox_logs->setGeometry(QRect(240, 330, 131, 31));
        checkBox_logs->setStyleSheet(QStringLiteral("font: 11pt \"Carlito\";"));
        label_tag = new QLabel(page);
        label_tag->setObjectName(QStringLiteral("label_tag"));
        label_tag->setGeometry(QRect(60, 120, 341, 20));
        label_tag->setAlignment(Qt::AlignCenter);
        line = new QFrame(page);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(70, 180, 311, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        plainTextEdit_msg = new QPlainTextEdit(page_2);
        plainTextEdit_msg->setObjectName(QStringLiteral("plainTextEdit_msg"));
        plainTextEdit_msg->setGeometry(QRect(10, 270, 341, 61));
        plainTextEdit_msg->setStyleSheet(QStringLiteral("background-color:rgb(255, 255, 255); border-radius: 5px;"));
        pushButton_sendmsg = new QPushButton(page_2);
        pushButton_sendmsg->setObjectName(QStringLiteral("pushButton_sendmsg"));
        pushButton_sendmsg->setGeometry(QRect(360, 270, 95, 61));
        pushButton_sendmsg->setStyleSheet(QStringLiteral("background-color: rgb(158, 204, 143); text-color: \"white\"; border-radius: 8px; text-color: \"white\";"));
        listWidget_messages = new QListWidget(page_2);
        listWidget_messages->setObjectName(QStringLiteral("listWidget_messages"));
        listWidget_messages->setGeometry(QRect(10, 31, 441, 231));
        listWidget_messages->setStyleSheet(QStringLiteral("background-color:rgb(255, 255, 255); border-radius: 5px;"));
        listWidget_messages->setFrameShadow(QFrame::Sunken);
        listWidget_messages->setAutoScroll(false);
        listWidget_messages->setAutoScrollMargin(0);
        listWidget_messages->setAlternatingRowColors(true);
        listWidget_messages->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        listWidget_messages->setMovement(QListView::Static);
        listWidget_messages->setProperty("isWrapping", QVariant(false));
        listWidget_messages->setResizeMode(QListView::Fixed);
        listWidget_messages->setWordWrap(true);
        label = new QLabel(page_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 81, 20));
        label->setStyleSheet(QStringLiteral("border-radius: 5px;"));
        label->setFrameShape(QFrame::StyledPanel);
        label->setFrameShadow(QFrame::Sunken);
        stackedWidget->addWidget(page_2);
        Chat->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(Chat);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Chat->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Chat);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Chat->setStatusBar(statusBar);
        menuBar = new QMenuBar(Chat);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 484, 26));
        Chat->setMenuBar(menuBar);

        retranslateUi(Chat);

        stackedWidget->setCurrentIndex(0);
        pushButton_chat->setDefault(false);


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
        checkBox_logs->setText(QApplication::translate("Chat", "Save Logs", 0));
        label_tag->setText(QApplication::translate("Chat", "The best linux chat client!", 0));
        pushButton_sendmsg->setText(QApplication::translate("Chat", "Send", 0));
        label->setText(QApplication::translate("Chat", "Messages", 0));
    } // retranslateUi

};

namespace Ui {
    class Chat: public Ui_Chat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHAT_H
