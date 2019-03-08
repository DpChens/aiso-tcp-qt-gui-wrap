#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "asiotcpserver.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_input_returnPressed();

private:

    Ui::Widget *ui;
    AsioTcpServer *server_;
    AsioTcpServer::sock_ptr socket_;
};

#endif // WIDGET_H
