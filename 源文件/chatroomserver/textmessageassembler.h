#ifndef TEXTMESSAGEASSEMBLER_H
#define TEXTMESSAGEASSEMBLER_H

#include <QObject>
#include <QString>
#include <QQueue>
#include <QSharedPointer>
#include <QByteArray>

#include "textmessgae.h"

class TextMessageAssembler : public QObject
{
    Q_OBJECT
public:
    explicit TextMessageAssembler(QObject *parent = nullptr);
    void prepare(const char *data, int len);
    QSharedPointer<TextMessgae> assemble();
    QSharedPointer<TextMessgae> assemble(const char *data, int len);
    void reset();

protected:
    void clear();
    QByteArray fetch(int n);
    bool makeTypeAndLength();
    TextMessgae *makeMessage();

protected:
    QString m_type;
    QByteArray m_data;
    int m_length = 0;
    QQueue<char> m_queue;
};

#endif // TEXTMESSAGEASSEMBLER_H
