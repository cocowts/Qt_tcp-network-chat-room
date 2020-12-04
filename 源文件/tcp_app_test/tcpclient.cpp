#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    connect(&m_socket, &QTcpSocket::readyRead, this, &TcpClient::slotReadyRead);
    connect(&m_socket, &QTcpSocket::disconnected, this, &TcpClient::slotDisconnected);
}

TcpClient::~TcpClient()
{
    close();
}

bool TcpClient::connectToHost(const QString &hostName, quint16 port)
{
    m_socket.connectToHost(hostName, port);

    return m_socket.waitForConnected();
}

void TcpClient::send(const TextMessgae &message)
{
    QByteArray ba = message.serialize();

    m_socket.write(ba.data(), ba.length());
}

qint64 TcpClient::bytesAvailable() const
{
    return m_socket.bytesAvailable();
}

void TcpClient::close()
{
    m_socket.close();
}

void TcpClient::setHandler(TextMessageHandler *handler)
{
    m_handler = handler;
}

void TcpClient::slotReadyRead()
{
    char buf[256] = {0};
    int len = 0;

    while ((len = static_cast<int>(m_socket.read(buf, sizeof(buf)))) > 0)
    {
        QSharedPointer<TextMessgae> ptm = m_assembler.assemble(buf, len);

        if ((ptm != nullptr) && (m_handler != nullptr))
        {
            m_handler->hanle(m_socket, *ptm);
        }
    }
}

void TcpClient::slotDisconnected()
{
    m_assembler.reset();
}
