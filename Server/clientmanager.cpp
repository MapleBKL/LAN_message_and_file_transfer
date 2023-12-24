#include "clientmanager.h"

ClientManager::ClientManager(QTcpSocket* client, QObject* parent)
    : QObject{parent},
    _client(client)
{
    connect(_client, &QTcpSocket::connected, this, &ClientManager::connected);
    connect(_client, &QTcpSocket::disconnected, this, &ClientManager::disconnected);
    connect(_client, &QTcpSocket::readyRead, this, &ClientManager::readyRead);
}

void ClientManager::uploadFile()
{
    _client->write(_protocol.setUploadMessage(_temp_filename));
}

void ClientManager::saveFile()
{
    QDir dir;
    dir.mkdir(username());
    auto path = QString("%1/%2/%3__%4").arg(dir.canonicalPath(), username(), QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"), _protocol.filename());
    QFile file(path);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(_protocol.filedata());
        file.close();
        emit fileSaved(path);
    }
}

void ClientManager::connectToServer()
{
    _client->connectToHost(_ip, _port);
}

void ClientManager::sendMessage(QString message)
{
    _client->write(_protocol.textMessage(message));
}

void ClientManager::sendIsTyping()
{
    _client->write(_protocol.isTypingMessage());
}

void ClientManager::sendName(QString name)
{
    _client->write(_protocol.setNameMessage(name));
}

void ClientManager::sendStatus(MessageProtocol::Status status)
{
    _client->write(_protocol.setStatusMessage(status));
}

QString ClientManager::username() const
{
    auto id = _client->property("id").toInt();
    auto name = _protocol.username().length() > 0 ? _protocol.username() : QString("Client_%1").arg(id);
    return name;
}

void ClientManager::sendRequestUpload(QString filename)
{
    // save the filename, since we may need to upload it later (if accepted)
    _temp_filename = filename;
    _client->write(_protocol.setRequestUploadMessage(filename));
}

void ClientManager::sendAcceptUpload()
{
    _client->write(_protocol.setAcceptUploadMessage());
}

void ClientManager::sendRejectUpload()
{
    _client->write(_protocol.setRejectUploadMessage());
}

void ClientManager::readyRead()
{
    auto data = _client->readAll();
    _protocol.parseData(data);
    switch (_protocol.type())
    {
    case MessageProtocol::Text:
        emit messageReceived(_protocol.message());
        break;
    case MessageProtocol::SetName:
        emit nameChanged(_protocol.username());
        break;
    case MessageProtocol::SetStatus:
        emit statusChanged(_protocol.status());
        break;
    case MessageProtocol::IsTyping:
        emit otherSideisTyping();
        break;
    case MessageProtocol::RequestUpload:
        emit fileRequestReceived(_protocol.username(), _protocol.filename(), _protocol.filesize());
        break;
    case MessageProtocol::AcceptUpload:
        uploadFile();
        break;
    case MessageProtocol::RejectUpload:
        emit fileRejected();
        break;
    case MessageProtocol::Upload:
        saveFile();
        break;
    default:
        break;
    }
}
