#include "tcpclient.h"

#include <QHostAddress>

TcpClient::TcpClient(QObject *parent) : QObject(parent), m_socket(this)
{
    connect(&m_socket, &QTcpSocket::readyRead, this, &TcpClient::slotReadyRead);
    connect(&m_socket, &QTcpSocket::disconnected, this, &TcpClient::slotDisconnected);
    connect(&m_socket, &QTcpSocket::connected, this, &TcpClient::slotConnected);
}

TcpClient::~TcpClient()
{
    m_socket.close();
}

bool TcpClient::connectToHost(const QString &hostName, quint16 port)
{
    m_socket.connectToHost(hostName, port);

    return m_socket.waitForConnected();
}

qint64 TcpClient::send(const TextMessgae &message)
{
    QByteArray ba = message.serialize();

    return m_socket.write(ba.data(), ba.length());
}

qint64 TcpClient::bytesAvailable() const
{
    return m_socket.bytesAvailable();
}

void TcpClient::close()
{
    m_socket.close();
}

bool TcpClient::isValid()
{
    return m_socket.state() == QAbstractSocket::ConnectedState;
}

void TcpClient::setHandler(TextMessageHandler *handler)
{
    m_handler = handler;
}

void TcpClient::slotConnected()
{
    if (m_handler != nullptr)
    {
        TextMessgae conn("CONN", m_socket.peerAddress().toString() + ":" + QString::number(m_socket.peerPort()));

        m_handler->hanle(m_socket, conn);
    }
}

void TcpClient::slotReadyRead()
{
    char buf[256] = {0};
    int len = 0;

    while ((len = static_cast<int>(m_socket.read(buf, sizeof(buf)))) > 0)
    {
        QSharedPointer<TextMessgae> ptm = nullptr;
        m_assembler.prepare(buf, len);

        while ((ptm = m_assembler.assemble()) != nullptr)
        {
            if (m_handler != nullptr)
            {
                m_handler->hanle(m_socket, *ptm);
            }
        }
    }
}

void TcpClient::slotDisconnected()
{
    m_assembler.reset();

    if (m_handler != nullptr)
    {
        TextMessgae dscn("DSCN", "");

        m_handler->hanle(m_socket, dscn);
    }
}
