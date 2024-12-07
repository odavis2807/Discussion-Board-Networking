#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString post = ui->txtPost->text();
    ui->txtPost->setText(NULL);

    std::string stdpost = post.toStdString();
    SendPost(stdpost);
}

void MainWindow::PostReveived(std::string post) {
    QString qpost = QString::fromStdString(post);
    ui->listBoard->insertItem(0, qpost);
}

void MainWindow::SendPost(std::string post) {
    //yet to be implemented
    qDebug() << "Post Sent: " << QString::fromStdString(post) << "\n";
}

