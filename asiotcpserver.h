#ifndef ASIOTCPSERVER_H
#define ASIOTCPSERVER_H

#include <QObject>
#include <QByteArray>

#include <asio.hpp>
#include <memory>
#include <functional>
#include <vector>
#include <utility>
using namespace std;
using namespace asio;
using namespace std::placeholders;
class AsioTcpServer : public QObject
{
    Q_OBJECT
public:
    typedef ip::tcp::socket socket_type;
    typedef shared_ptr<socket_type> sock_ptr;
    typedef shared_ptr<array<char, 1024> > buf_ptr;
    typedef pair<sock_ptr, buf_ptr> sock_buf;

public:
    AsioTcpServer(quint16 port = 0, QObject *parent = Q_NULLPTR);
    void start();
    sock_ptr nextPendingConnection();
    void write(sock_ptr sock, const char *data, qint64 len);
private:
    void accept();//监听

    void acceptHandler(const std::error_code& ec, sock_ptr sock, buf_ptr buf);
    void writeHandler(const std::error_code& ec);
    void readHandler(const std::error_code& ec, size_t bytes_transferred, sock_ptr sock, buf_ptr buf);
signals:
    void newConnection();
    void readyRead(QByteArray data);

private:
    io_service m_io;
    ip::tcp::acceptor m_acceptor;
    vector<sock_buf> sock_buf_;
};

#endif // ASIOTCPSERVER_H
