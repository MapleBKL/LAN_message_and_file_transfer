#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QDir>
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
    void saveFile();

public:
    explicit ClientManager(QTcpSocket* client, QObject* parent = nullptr);
    void connectToServer();
    void sendMessage(QString message);
    void sendIsTyping();
    void sendName(QString name);
    void sendStatus(MessageProtocol::Status status);

    QString username() const;

    void sendRequestUpload(QString filename);
    void sendAcceptUpload();
    void sendRejectUpload();

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
    void fileSaved(QString filepath);

private slots:
    void readyRead();
};

#endif // CLIENTMANAGER_H
