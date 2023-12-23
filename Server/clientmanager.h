#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QTcpSocket>
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
    explicit ClientManager(QTcpSocket* client, QObject* parent = nullptr);
    void connectToServer();
    void sendMessage(QString message);
    void sendIsTyping();
    void sendName(QString name);
    void sendStatus(MessageProtocol::Status status);

    QString name() const;

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
