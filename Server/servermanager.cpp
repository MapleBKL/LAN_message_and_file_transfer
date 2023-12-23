#include "servermanager.h"

ServerManager::ServerManager(QObject* parent)
    : QObject{parent}
{
    _server = new QTcpServer(this);
    connect(_server, &QTcpServer::newConnection, this, &ServerManager::newClientConnectionReceived);
}

void ServerManager::newClientConnectionReceived()
{
    // Create a TCP socket for the new client
    auto client = _server->nextPendingConnection();
    // assign an ID
    auto id = _clients.size();
    client->setProperty("id", id);
    // add to the client list
    _clients << client;

    connect(client, &QTcpSocket::disconnected, this, &ServerManager::onClientDisconnected);
    emit newClientConnected(client);
}

void ServerManager::onClientDisconnected()
{
    auto client = qobject_cast<QTcpSocket*>(sender());
    // remove the client (signal sender) from the client list
    _clients.removeOne(client);
    emit clientDisconnected(client);
}

/* if server is listening, return 1
 * if server is not listening, return 0
 * if listening error occurred, return -1 */
ushort ServerManager::toggleServer(QHostAddress serverAddress, ushort port)
{
    if (_server->isListening())
    {
        // if server is currently listening, stop listening
        _server->close();
        return 0;
    }
    else
    {
        // if server is not listening, start listening
        if (_server->listen(serverAddress, port))
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
}
