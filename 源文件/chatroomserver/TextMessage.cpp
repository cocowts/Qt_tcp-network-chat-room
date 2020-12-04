#include "TextMessage.h"


TextMessage::TextMessage(QObject* parent) : QObject(parent)
{
    m_type = "";
    m_data = "";
}

TextMessage::TextMessage(QString type, QString data, QObject* parent) : QObject(parent)
{
    m_type = type.trimmed();

    m_type.resize(4, ' ');

    m_data = data.mid(0, 15000);
}

QString TextMessage::type()
{
    return m_type.trimmed();
}

int TextMessage::length()
{
    return m_data.length();
}

QString TextMessage::data()
{
    return m_data;
}

QByteArray TextMessage::serialize()
{
    QByteArray ret;
    QByteArray dba = m_data.toUtf8();
    QString len = QString::asprintf("%X", dba.length());

    len.resize(4, ' ');

    ret.append(m_type.toStdString().c_str(), 4);
    ret.append(len.toStdString().c_str(), 4);
    ret.append(dba);

    return ret;
}

bool TextMessage::unserialize(QByteArray ba)
{
    bool ret = (ba.length() >= 8);

    if( ret )
    {
        QString type = QString(ba.mid(0, 4));
        QString len = QString(ba.mid(4, 4)).trimmed();
        int l = len.toInt(&ret, 16);

        ret = ret && (l == (ba.length() - 8));

        if( ret )
        {
            m_type = type;
            m_data = QString(ba.mid(8));
        }
    }

    return ret;
}
