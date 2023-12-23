#include "clientmanager.h"

ClientManager::ClientManager(QTcpSocket* client, QObject* parent)
    : QObject{parent},
    _client(client)
{
    connect(_client, &QTcpSocket::connected, this, &ClientManager::connected);
    connect(_client, &QTcpSocket::disconnected, this, &ClientManager::disconnected);
    connect(_client, &QTcpSocket::readyRead, this, &ClientManager::readyRead);
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

QString ClientManager::name() const
{
    auto id = _client->property("id").toInt();
    auto name = _protocol.name().length() > 0 ? _protocol.name() : QString("Client_%1").arg(id);
    return name;
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
