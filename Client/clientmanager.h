#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "messageprotocol.h"

class ClientManager : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* _client;
    QHostAddress _ip;
    ushort _port;
    MessageProtocol _protocol;

public:
    explicit ClientManager(QHostAddress ip, ushort port, QObject* parent = nullptr);
    void connectToServer();
    void disconnectFromServer();
    bool isConnected();
    void sendMessage(QString message);
    void sendIsTyping();
    void sendName(QString name);
    void sendStatus(MessageProtocol::Status status);

signals:
    void connected();
    void disconnected();
    // message protocol signals
    void messageReceived(QString message);
    void nameChanged(QString name);
    void statusChanged(MessageProtocol::Status status);
    void otherSideisTyping();

private slots:
    void readyRead();
};

#endif // CLIENTMANAGER_H
