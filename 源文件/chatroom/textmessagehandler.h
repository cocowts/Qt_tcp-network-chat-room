#ifndef TEXTMESSAGEHANDLER_H
#define TEXTMESSAGEHANDLER_H

#include <QTcpSocket>
#include "textmessgae.h"

class TextMessageHandler
{
public:
    virtual void hanle(QTcpSocket &socket, TextMessgae &message) = 0;
};

#endif // TEXTMESSAGEHANDLER_H
