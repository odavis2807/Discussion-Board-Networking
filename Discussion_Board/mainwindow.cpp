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
    if (!ui->txtAuthor->isModified()) {
        ui->txtAuthor->setText("REQUIRED");
    }
    if (!ui->txtTopic->isModified()) {
        ui->txtTopic->setText("REQUIRED");
    }
    if (!ui->txtPost->isModified()) {
        ui->txtPost->setText("REQUIRED");
    }
    if(ui->txtAuthor->isModified() && ui->txtTopic->isModified() && ui->txtPost->isModified()) {
        std::string stdpost = "POST|" + ui->txtAuthor->text().toStdString() + "|" + ui->txtTopic->text().toStdString() + "|" + ui->txtPost->text().toStdString();
        SendPost(stdpost);
    }


}

void MainWindow::PostReveived(std::string post) {
    std::vector<std::string> parsedPost = parsePost(post);

    QString qpost = "\nAuthor: " + QString::fromStdString(parsedPost[1]) + "\nTopic: " + QString::fromStdString(parsedPost[2]) + "\nPost:\n" + QString::fromStdString(parsedPost[3]);
    ui->listBoard->insertItem(0, qpost);
}

void MainWindow::SendPost(std::string post) {
    //yet to be implemented
    qDebug() << "Post Sent: " << QString::fromStdString(post) << "\n";
}

std::vector<std::string> MainWindow::parsePost(std::string post) {
    std::vector<std::string> parsedPost;
    std::stringstream ss(post);
    std::string segment;

    while (std::getline(ss, segment, '|')) {
        parsedPost.push_back(segment);
    }
    return parsedPost;
}

