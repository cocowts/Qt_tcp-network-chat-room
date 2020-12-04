#include "textmessageassembler.h"

TextMessageAssembler::TextMessageAssembler(QObject *parent) : QObject(parent)
{ }

void TextMessageAssembler::clear()
{
    m_type.clear();
    m_data.clear();
    m_length = 0;
}

void TextMessageAssembler::reset()
{
    clear();
    m_queue.clear();
}

QByteArray TextMessageAssembler::fetch(int n)
{
    QByteArray ret;

    for (int i=0; i<n; ++i)
    {
        ret += m_queue.dequeue();
    }

    return ret;
}

bool TextMessageAssembler::makeTypeAndLength()
{
   bool ret = m_queue.size() >= 8;

   if (ret)
   {
       m_type = QString::fromLatin1(fetch(4));
       m_length = QString::fromLatin1(fetch(4)).trimmed().toInt(&ret, 16);

       if (!ret)
       {
           clear();
       }
   }

   return ret;
}

TextMessgae *TextMessageAssembler::makeMessage()
{
    TextMessgae *ret = nullptr;

    if (!m_type.isEmpty())
    {
        int needed = m_length - m_data.length();
        int n = (needed <= m_queue.length()) ? needed : m_queue.length();

        m_data += fetch(n);

        if (m_data.length() == m_length)
        {
            ret = new TextMessgae(m_type, m_data);
        }
    }

    return ret;
}

void TextMessageAssembler::prepare(const char *data, int len)
{
    if (data != nullptr)
    {
        for (int i=0; i<len; ++i)
        {
            m_queue.enqueue(data[i]);
        }
    }
}

QSharedPointer<TextMessgae> TextMessageAssembler::assemble()
{
    TextMessgae *ret = nullptr;
    bool tryMakeMsg = true;

    if (m_type.isEmpty())
    {
        tryMakeMsg = makeTypeAndLength();
    }

    if (tryMakeMsg)
    {
        ret = makeMessage();
    }

    if (ret != nullptr)
    {
        clear();
    }

    return QSharedPointer<TextMessgae>(ret);
}

QSharedPointer<TextMessgae> TextMessageAssembler::assemble(const char *data, int len)
{
    prepare(data, len);

    return assemble();
}
