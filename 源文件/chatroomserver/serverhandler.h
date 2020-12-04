#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QObject>
#include <QList>

#include "textmessagehandler.h"

class ServerHandler : public QObject, public TextMessageHandler
{
    Q_OBJECT

protected:

    struct Node
    {
        QString id;
        QString password;
        QString status;
        QString level;
        QTcpSocket *socket = nullptr;
    };

public:
    explicit ServerHandler(QObject *parent = nullptr);
    ~ServerHandler() override;
    void hanle(QTcpSocket &socket, TextMessgae &message) override;

protected:
    QString getOnlineUserId();
    void sendToAllOnlineUser(const TextMessgae &message);

    void CONN_handler(QTcpSocket &socket, TextMessgae &message);
    void DSCN_handler(QTcpSocket &socket, TextMessgae &message);
    void LIGN_handler(QTcpSocket &socket, TextMessgae &message);
    void MSGA_handler(QTcpSocket &socket, TextMessgae &message);
    void MSGP_handler(QTcpSocket &socket, TextMessgae &message);
    void ADMN_handler(QTcpSocket &socket, TextMessgae &message);

protected:
    QList<Node*> m_nodeList;
};

#endif // SERVERHANDLER_H
