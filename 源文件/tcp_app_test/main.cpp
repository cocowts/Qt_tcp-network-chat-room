#include <QCoreApplication>
#include <QDebug>

#include "tcpclient.h"
#include "tcpserver.h"
#include "textmessgae.h"
#include "textmessageassembler.h"
#include "textmessagehandler.h"

class Handler : public TextMessageHandler
{
public:
    void hanle(QTcpSocket &socket, TextMessgae &message) override
    {
        qDebug() << &socket;
        qDebug() << message.type();
        qDebug() << message.length();
        qDebug() << message.data();
    }

    virtual ~Handler()
    {    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TextMessgae tt("Demo123", "hello 你好");

    Handler handler;
    TcpClient client;
    TcpServer server;

    server.setHandler(&handler);
    server.listen(8890);

    client.setHandler(&handler);
    client.connectToHost("127.0.0.1", 8890);

    client.send(tt);

    return a.exec();
}
