#ifndef CLIENTCHATWIDGET_H
#define CLIENTCHATWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QDateTime>
#include <QMessageBox>
#include "clientmanager.h"

namespace Ui {
class ClientChatWidget;
}

class ClientChatWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::ClientChatWidget* ui;
    ClientManager*        _client;

public:
    explicit ClientChatWidget(QTcpSocket* client, QWidget* parent = nullptr);
    ~ClientChatWidget();

signals:
    void clientNameChanged(QString name);
    void clientStatusChanged(MessageProtocol::Status status);
    void clientIsTyping(QString name);

private slots:
    void on_btn_send_clicked();
    void clientDisconnected();
    // message related slots
    void messageReceived(QString message);
    void nameChanged(QString name);
    void statusChanged(MessageProtocol::Status status);
    void onTyping();
    // file related slots
    void handleFileRequest(QString username, QString filename, qint64 filesize);
    void onFileSaved(QString path);
};

#endif // CLIENTCHATWIDGET_H
