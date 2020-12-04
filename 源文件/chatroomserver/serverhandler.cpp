#include "serverhandler.h"

ServerHandler::ServerHandler(QObject *parent) : QObject(parent)
{
    Node *admin = new Node();

    admin->id = "admin";
    admin->password = "admin";
    admin->level = "admin";

    m_nodeList.append(admin);
}

ServerHandler::~ServerHandler()
{
    for (auto &item : m_nodeList)
    {
        delete item;
    }
}

QString ServerHandler::getOnlineUserId()
{
    QString ret;

    for (const auto &item : m_nodeList)
    {
        if (item->socket != nullptr)
        {
            ret += item->id + '\r';
        }
    }

    return ret;
}

void ServerHandler::sendToAllOnlineUser(const TextMessgae &message)
{
    const QByteArray &ba = message.serialize();

    for (const auto &item : m_nodeList)
    {
        if (item->socket != nullptr)
        {
            item->socket->write(ba);
        }
    }
}

void ServerHandler::CONN_handler(QTcpSocket &socket, TextMessgae &message)
{
    Q_UNUSED(socket)
    Q_UNUSED(message)
}

void ServerHandler::DSCN_handler(QTcpSocket &socket, TextMessgae &message)
{
    Q_UNUSED(message)

    Node *node = nullptr;

    for (int i=0; i<m_nodeList.length(); ++i)
    {
        node = m_nodeList.at(i);

        if (node->socket == &socket)
        {
            node->socket = nullptr;
            break;
        }
    }

    sendToAllOnlineUser(TextMessgae("USER", getOnlineUserId()));

    if (node != nullptr)
    {
        QString text = QString("<html> \
               <head>   \
               <style>  \
               font{color:gray;} #f{font-size:25px; color: green;} \
               </style> \
               </head>  \
               <body>   \
               <font>%1<br></font><font id=\"f\">%2</font><br> \
               </html>").arg(QString("[System]") + ":\n").arg("========= [ " + node->id + " ] Leave chat room ========");

        sendToAllOnlineUser(TextMessgae("MSGA", text));
    }
}

void ServerHandler::LIGN_handler(QTcpSocket &socket, TextMessgae &message)
{
    QString data = message.data();
    int index = data.indexOf('\r');
    QString id = data.mid(0, index);
    QString password = data.mid(index + 1);
    QString result;
    QString status;
    QString level;

    index = -1;
    for (int i=0; i<m_nodeList.length(); ++i)
    {
        if (id == m_nodeList.at(i)->id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        Node *node = new Node();

        if (node != nullptr)
        {
            node->id = id;
            node->password = password;
            node->socket = &socket;

            m_nodeList.append(node);

            result = "LIOK";

            status = node->status;
            level = node->level;
        }
        else
        {
            result = "LIER";
        }
    }
    else
    {
        Node *node = m_nodeList.at(index);

        if (password == node->password)
        {
            node->socket = &socket;

            result = "LIOK";

            status = node->status;
            level = node->level;
        }
        else
        {
            result = "LIER";
        }
    }

    socket.write(TextMessgae(result, id + '\r' + status + '\r' + level).serialize());

    if (result == "LIOK")
    {
        sendToAllOnlineUser(TextMessgae("USER", getOnlineUserId()));

        QString text = QString("<html> \
               <head>   \
               <style>  \
               font{color:gray;} #f{font-size:25px; color: green;} \
               </style> \
               </head>  \
               <body>   \
               <font>%1<br></font><font id=\"f\">%2</font><br> \
               </html>").arg(QString("[System]") + ":\n").arg("========= [ " + id + " ] Enter chat room ========");

        sendToAllOnlineUser(TextMessgae("MSGA", text));
    }
}

void ServerHandler::MSGA_handler(QTcpSocket &socket, TextMessgae &message)
{
    Q_UNUSED(socket)

    const QByteArray &ba = message.serialize();

    for (int i=0; i<m_nodeList.length(); ++i)
    {
        Node *node = m_nodeList.at(i);

        if (node->socket != nullptr)
        {
            node->socket->write(ba);
        }
    }
}

void ServerHandler::MSGP_handler(QTcpSocket &socket, TextMessgae &message)
{
    Q_UNUSED(socket)

    QStringList tl = message.data().split("\r", QString::SkipEmptyParts);
    const QByteArray &ba = TextMessgae("MSGA", tl.last()).serialize();

    tl.removeLast();

    for (const auto &id : tl)
    {
        for (const auto &node : m_nodeList)
        {
            if ((node->socket != nullptr) && (node->id == id))
            {
                node->socket->write(ba);
                break;
            }
        }
    }
}

void ServerHandler::ADMN_handler(QTcpSocket &socket, TextMessgae &message)
{
    Q_UNUSED(socket)

    QStringList data = message.data().split("\r", QString::SkipEmptyParts);
    QString op = data[0];
    QString id = data[1];

    for (const auto &node : m_nodeList)
    {
        if ((id == node->id) && (node->socket != nullptr) && (node->level != "admin"))
        {
            node->socket->write(TextMessgae("CTRL", op).serialize());
            node->status = op;
            break;
        }
    }
}

void ServerHandler::hanle(QTcpSocket &socket, TextMessgae &message)
{
    if (message.type() == "CONN")
        CONN_handler(socket, message);
    else if (message.type() == "DSCN")
        DSCN_handler(socket, message);
    else if (message.type() == "LIGN")
        LIGN_handler(socket, message);
    else if (message.type() == "MSGA")
        MSGA_handler(socket, message);
    else if (message.type() == "MSGP")
        MSGP_handler(socket, message);
    else if (message.type() == "ADMN")
        ADMN_handler(socket, message);
}


