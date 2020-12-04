#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

#include "textmessageassembler.h"
#include "textmessagehandler.h"
#include "textmessgae.h"

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient();
    bool connectToHost(const QString &hostName, quint16 port);
    void send(const TextMessgae &message);
    qint64 bytesAvailable() const;
    void close();

    void setHandler(TextMessageHandler *handler);

protected slots:
    void slotReadyRead();
    void slotDisconnected();

protected:
    QTcpSocket m_socket;
    TextMessageAssembler m_assembler;
    TextMessageHandler *m_handler = nullptr;
};

#endif // TCPCLIENT_H
