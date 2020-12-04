#ifndef TEXTMESSGAE_H
#define TEXTMESSGAE_H

#include <QObject>
#include <QString>
#include <QByteArray>

class TextMessgae : public QObject
{
    Q_OBJECT
public:
    explicit TextMessgae(QObject *parent = nullptr);
    explicit TextMessgae(const QString &type, const QString &data, QObject *parent = nullptr);
    TextMessgae(const TextMessgae&) = delete;
    TextMessgae &operator= (const TextMessgae&) = delete;
    QString type() const;
    int length() const;
    QString data() const;
    QByteArray serialize() const;
    bool unserialize(const QByteArray &msg);

protected:
    QString m_type;
    QString m_data;
};

#endif // TEXTMESSGAE_H
