#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_chat_clicked()
{
    username = ui->textEdit_username->toPlainText();
    ui->listWidget_users->addItem(username);
    ui->listWidget->addItem("Connected!");
    ui->listWidget->addItem("Welcome, " + username + "!");
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_clicked()
{
    ui->listWidget->addItem(username + ": " + ui->textEdit->toPlainText());
}
