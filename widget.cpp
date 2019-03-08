#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QByteArray>
#include <thread>
#include <iostream>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    server_(nullptr)
{
    ui->setupUi(this);
    server_ = new AsioTcpServer(8860);
    QObject::connect(server_, &AsioTcpServer::newConnection, [this](){
        qDebug() << "新的连接";
        std::cout << "tcp thread id: " << this_thread::get_id() << std::endl;
        socket_ = server_->nextPendingConnection();
    });

    QObject::connect(server_, &AsioTcpServer::readyRead, [this](QByteArray data){
        std::cout << "tcp thread id: " << this_thread::get_id() << std::endl;
        ui->plainTextEdit->insertPlainText(QString("接受: %1\n").arg(QString(data)));
        ui->input->setFocus();
    });
    std::thread thd([this]{server_->start();});
    thd.detach();
    std::cout << "main thread id: " << this_thread::get_id() << std::endl;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_input_returnPressed()
{
    if(socket_.get() == nullptr){
        ui->plainTextEdit->insertPlainText("socket还未连接");
        return;
    }

    QString message = ui->input->text();
    server_->write(socket_, message.toStdString().c_str(), strlen(message.toStdString().c_str()));
    ui->input->clear();
    ui->plainTextEdit->insertPlainText(QString("我: %1\n").arg(message));
}
