#ifndef MESSAGEPROTOCOL_H
#define MESSAGEPROTOCOL_H

#include <QByteArray>
#include <QString>
#include <QIODevice>

class MessageProtocol
{
public:
    enum MessageType
    {
        Text,
        File,
        IsTyping,
        SetName,
        SetStatus
    };

    enum Status
    {
        Available,
        Away,
        Busy
    };

    MessageProtocol();

    QByteArray textMessage(QString message);
    QByteArray isTypingMessage();
    QByteArray setNameMessage(QString name);
    QByteArray setStatusMessage(Status status);

    void parseData(QByteArray data);

    QString message() const;
    QString name() const;
    Status status() const;
    MessageType type() const;

private:
    MessageType _type;
    QString     _message;
    QString     _name;
    Status      _status;

    QByteArray convertData(MessageType type, QString data);
};

#endif // MESSAGEPROTOCOL_H
