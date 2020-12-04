#include "textmessgae.h"

TextMessgae::TextMessgae(QObject *parent) : QObject(parent)
{ }

TextMessgae::TextMessgae(const QString &type, const QString &data, QObject *parent) : QObject(parent)
{
    m_type = type.trimmed().mid(0, 4);

    m_type.resize(4, ' ');

    m_data = data.mid(0, 0xFFFF);
}

QString TextMessgae::type() const
{
    return m_type;
}

int TextMessgae::length() const
{
    return m_data.length();
}

QString TextMessgae::data() const
{
    return m_data;
}

QByteArray TextMessgae::serialize() const
{
    QByteArray ret;

    QByteArray data = m_data.toUtf8();

    QString len = QString::asprintf("%X", data.length());
    len.resize(4, ' ');

    ret.append(m_type.toLatin1());
    ret.append(len.toLatin1());
    ret.append(data);

    return ret;
}

bool TextMessgae::unserialize(const QByteArray &msg)
{
    bool ret = msg.length() >= 8;

    if (ret)
    {
        QString type = QString::fromLatin1(msg.mid(0, 4), 4);
        QString len = QString::fromLatin1(msg.mid(4, 4), 4);
        int l = len.toInt(&ret, 16);

        ret = ret && (l = msg.length() - 8);

        if (ret)
        {
            m_type = type;
            m_data = QString::fromUtf8(msg.mid(8));
        }
    }

    return ret;
}
