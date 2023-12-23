#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class ServerManager : public QObject
{
    Q_OBJECT
private:
// fields
    QTcpServer* _server;
    QList<QTcpSocket*> _clients;

// functions

public:
    explicit ServerManager(QObject* parent = nullptr);
    ushort toggleServer(QHostAddress serverAddress, ushort port);

private slots:
    void newClientConnectionReceived();
    void onClientDisconnected();

signals:
    void newClientConnected(QTcpSocket* client);
    void clientDisconnected(QTcpSocket* client);
};

#endif // SERVERMANAGER_H
