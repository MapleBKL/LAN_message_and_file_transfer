#include "clientmanager.h"

ClientManager::ClientManager(QObject *parent)
    : QObject{parent}
{
    _client = new QTcpSocket(this);
    connect(_client, &QTcpSocket::connected, this, &ClientManager::connected);
    connect(_client, &QTcpSocket::disconnected, this, &ClientManager::disconnected);
    connect(_client, &QTcpSocket::readyRead, this, &ClientManager::readyRead);
}

ClientManager::ClientManager(QHostAddress ip, ushort port, QObject* parent)
    : QObject{parent},
    _ip(ip),
    _port(port)
{
    _client = new QTcpSocket(this);
    connect(_client, &QTcpSocket::connected, this, &ClientManager::connected);
    connect(_client, &QTcpSocket::disconnected, this, &ClientManager::disconnected);
    connect(_client, &QTcpSocket::readyRead, this, &ClientManager::readyRead);
}

QHostAddress ClientManager::ip() const
{
    return _ip;
}

void ClientManager::setIp(const QHostAddress &newIp)
{
    _ip = newIp;
}

ushort ClientManager::port() const
{
    return _port;
}

void ClientManager::setPort(ushort newPort)
{
    _port = newPort;
}

void ClientManager::uploadFile()
{
    _client->write(_protocol.setUploadMessage(_temp_filename));
}

void ClientManager::connectToServer()
{
    _client->connectToHost(_ip, _port);
}

void ClientManager::disconnectFromServer()
{
    _client->disconnectFromHost();
}

bool ClientManager::isConnected()
{
    return _client->state() == QAbstractSocket::ConnectedState;
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
    default:
        break;
    }
}
