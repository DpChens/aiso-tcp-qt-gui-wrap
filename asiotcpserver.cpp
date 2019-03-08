#include "asiotcpserver.h"
#include <iostream>
AsioTcpServer::AsioTcpServer(quint16 port, QObject *parent):
    QObject(parent),
    m_acceptor(m_io, ip::tcp::endpoint(ip::tcp::v4(), port))
{
    accept();
}

void AsioTcpServer::start()
{
    m_io.run();
}

AsioTcpServer::sock_ptr AsioTcpServer::nextPendingConnection()
{
    return  sock_buf_.back().first;
}

void AsioTcpServer::accept()
{
    sock_ptr sock(new socket_type(m_io));
    buf_ptr  buf(new array<char, 1024>);
    sock_buf_.push_back(make_pair(sock, buf) );
    m_acceptor.async_accept(*sock, bind(&AsioTcpServer::acceptHandler, this, _1, sock, buf));
}

void AsioTcpServer::acceptHandler(const error_code &ec, AsioTcpServer::sock_ptr sock, AsioTcpServer::buf_ptr buf)
{
    if(ec){
        cerr << __FUNCTION__ << "has some error: " << ec;
        return;
    }
    emit newConnection();
    sock->async_read_some(buffer(*buf), bind(&AsioTcpServer::readHandler, this, _1, _2, sock, buf));
    accept();
}

void AsioTcpServer::write(sock_ptr sock, const char *data, qint64 len)
{
    sock->async_write_some(buffer(data, len), bind(&AsioTcpServer::writeHandler, this, _1));

}

void AsioTcpServer::writeHandler(const error_code &ec)
{
    cout << "send msg complete." << endl;
}
void AsioTcpServer::readHandler(const error_code &ec, size_t bytes_transferred, AsioTcpServer::sock_ptr sock, AsioTcpServer::buf_ptr buf)
{
    (*buf).data()[bytes_transferred] = '\0';
    emit  readyRead(QByteArray::fromRawData((*buf).data(), bytes_transferred));
    sock->async_read_some(buffer(*buf), bind(&AsioTcpServer::readHandler, this, _1, _2, sock, buf)); //必须再调用
}
