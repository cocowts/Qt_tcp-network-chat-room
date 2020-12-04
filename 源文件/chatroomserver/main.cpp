#include <QCoreApplication>

#include "tcpserver.h"
#include "serverhandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TcpServer server(qApp);
    ServerHandler handler(qApp);

    server.setHandler(&handler);
    server.listen(8890);

    return a.exec();
}
