#include "messageprotocol.h"

MessageProtocol::MessageProtocol()
{

}

QByteArray MessageProtocol::textMessage(QString message)
{
    return convertData(Text, message);
}

QByteArray MessageProtocol::isTypingMessage()
{
    return convertData(IsTyping, "");
}

QByteArray MessageProtocol::setNameMessage(QString name)
{
    return convertData(SetName, name);
}

QByteArray MessageProtocol::setStatusMessage(Status status)
{
    QByteArray bytearray;
    QDataStream out(&bytearray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << SetStatus << status;
    return bytearray;
}

void MessageProtocol::parseData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_6_0);
    in >> _type;
    switch (_type) {
    case Text:
        in >> _message;
        break;
    case SetName:
        in >> _name;
        break;
    case SetStatus:
        in >> _status;
        break;
    default:
        break;
    }
}

QString MessageProtocol::message() const
{
    return _message;
}

QString MessageProtocol::name() const
{
    return _name;
}

MessageProtocol::Status MessageProtocol::status() const
{
    return _status;
}

MessageProtocol::MessageType MessageProtocol::type() const
{
    return _type;
}

QByteArray MessageProtocol::convertData(MessageType type, QString data)
{
    QByteArray bytearray;
    QDataStream out(&bytearray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << type << data;
    return bytearray;
}
