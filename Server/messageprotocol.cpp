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

QByteArray MessageProtocol::setRequestUploadMessage(QString filename)
{
    QByteArray bytearray;
    QDataStream out(&bytearray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    QFileInfo info(filename);
    out << RequestUpload << info.fileName() << info.size();
    return bytearray;
}

QByteArray MessageProtocol::setAcceptUploadMessage()
{
    return convertData(AcceptUpload, "");
}

QByteArray MessageProtocol::setRejectUploadMessage()
{
    return convertData(RejectUpload, "");
}

QByteArray MessageProtocol::setUploadMessage(QString filename)
{
    QByteArray bytearray;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream out(&bytearray, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_0);
        QFileInfo info(filename);
        out << Upload << info.fileName() << info.size() << file.readAll();
        file.close();
    }
    return bytearray;
}

// parse the data received from the socket
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
        in >> _username;
        break;
    case SetStatus:
        in >> _status;
        break;
    case RequestUpload:
        in >> _filename >> _filesize;
        break;
    case Upload:
        in >> _filename >> _filesize >> _filedata;
        break;
    default:
        break;
    }
}

// getter functions
QString MessageProtocol::message() const
{
    return _message;
}

QString MessageProtocol::username() const
{
    return _username;
}

QString MessageProtocol::filename() const
{
    return _filename;
}

qint64 MessageProtocol::filesize() const
{
    return _filesize;
}

QByteArray MessageProtocol::filedata() const
{
    return _filedata;
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
