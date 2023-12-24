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
    QTcpSocket*     _client;
    QHostAddress    _ip;
    ushort          _port;
    MessageProtocol _protocol;
    QString         _temp_filename;

    void uploadFile();

public:
    explicit ClientManager(QObject* parent = nullptr);
    explicit ClientManager(QHostAddress ip, ushort port, QObject* parent = nullptr);

    void connectToServer();
    void disconnectFromServer();
    bool isConnected();

    void sendMessage(QString message);
    void sendIsTyping();
    void sendName(QString name);
    void sendStatus(MessageProtocol::Status status);

    void sendRequestUpload(QString filename);
    void sendAcceptUpload();
    void sendRejectUpload();

    // getter and setter for ip
    QHostAddress ip() const;
    void setIp(const QHostAddress &newIp);

    // getter and setter for port
    ushort port() const;
    void setPort(ushort newPort);

signals:
    void connected();
    void disconnected();
    // message protocol signals
    void messageReceived(QString message);
    void nameChanged(QString name);
    void statusChanged(MessageProtocol::Status status);
    void otherSideisTyping();
    // file protocol signals
    void fileRejected();
    void fileRequestReceived(QString username, QString filename, qint64 filesize);

private slots:
    void readyRead();
};

#endif // CLIENTMANAGER_H
