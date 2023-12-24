#include "clientmanager.h"

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
        emit nameChanged(_protocol.name());
        break;
    case MessageProtocol::SetStatus:
        emit statusChanged(_protocol.status());
        break;
    case MessageProtocol::IsTyping:
        emit otherSideisTyping();
        break;
    default:
        break;
    }
}
