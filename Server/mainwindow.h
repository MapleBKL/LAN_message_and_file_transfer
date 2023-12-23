#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkInterface>
#include <QMessageBox>
#include "servermanager.h"
#include "messageprotocol.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    // fields
    Ui::MainWindow* ui;
    ServerManager* _server_manager;

    // functions
    void setupServerManager();
    void listIpAddresses();

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void newClientConnected(QTcpSocket* client);
    void clientDisconnected(QTcpSocket* client);
    void on_btn_listen_clicked();
    void setClientName(QString name);
    void setClientStatus(MessageProtocol::Status status);
};
#endif // MAINWINDOW_H
