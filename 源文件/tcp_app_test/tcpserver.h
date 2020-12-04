#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>

#include "textmessageassembler.h"
#include "textmessagehandler.h"
#include "textmessgae.h"

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();
    bool listen(quint16 port);
    void close();
    void setHandler(TextMessageHandler *handler);

protected slots:
    void slotNewConnection();
    void slotSocketReadyRead();
    void slotSocketDisconnected();

protected:
    QTcpServer m_server;
    QMap<QTcpSocket*, TextMessageAssembler*> m_map;
    TextMessageHandler *m_handler = nullptr;
};

#endif // TCPSERVER_H
