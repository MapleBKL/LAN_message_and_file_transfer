#include "mainwindow.h"
#include "clientchatwidget.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    listIpAddresses();
    setupServerManager();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newClientConnected(QTcpSocket* client)
{
    auto id = client->property("id").toInt();
    ui->client_list->addItem(QString("New connection: %1").arg(id));
    auto chatWidget = new ClientChatWidget(client);
    ui->message_tabs->addTab(chatWidget, QString("Client_%1").arg(id));
    ui->message_tabs->setTabIcon(ui->message_tabs->indexOf(chatWidget), QIcon(":/icons/available.png"));

    connect(chatWidget, &ClientChatWidget::disconnected, this, &MainWindow::onWidgetDisconnected);
    connect(chatWidget, &ClientChatWidget::clientNameChanged, this, &MainWindow::setClientName);
    connect(chatWidget, &ClientChatWidget::clientStatusChanged, this, &MainWindow::setClientStatus);
    connect(chatWidget, &ClientChatWidget::clientIsTyping, this, [this](QString name) {
        this->statusBar()->showMessage(QString("%1 is typing...").arg(name), 750);
    });
}

void MainWindow::clientDisconnected(QTcpSocket* client)
{
    auto id = client->property("id").toInt();
    ui->client_list->addItem(QString("Disconnected: %1").arg(id));
}

void MainWindow::onWidgetDisconnected()
{
    auto widget = qobject_cast<QWidget*>(sender());
    auto index = ui->message_tabs->indexOf(widget);
    auto icon = QIcon(":/icons/offline.png");
    ui->message_tabs->setTabIcon(index, icon);
}

void MainWindow::setupServerManager()
{
    _server_manager = new ServerManager();
    connect(_server_manager, &ServerManager::newClientConnected, this, &MainWindow::newClientConnected);
    connect(_server_manager, &ServerManager::clientDisconnected, this, &MainWindow::clientDisconnected);
}

void MainWindow::listIpAddresses()
{
    // for filtering out unique IPV4 addresses
    QSet<QString> ipList;

    for (const QNetworkInterface& interface : QNetworkInterface::allInterfaces())
    {
        for (const QHostAddress& addr : interface.allAddresses())
        {
            // only IPV4 addresses
            if (addr.protocol() == QAbstractSocket::IPv4Protocol)
            {
                ipList.insert(addr.toString());
            }
        }
    }
    // convert to a string list
    QStringList uniqueAddresses(ipList.begin(), ipList.end());
    ui->ip_combo_box->addItems(uniqueAddresses);
}

void MainWindow::on_btn_listen_clicked()
{
    // acquire IP address and port to listen on:
    // acquire current server IP address
    QString serverIpAddress = ui->ip_combo_box->currentText();
    // acquire current port
    ushort port = ui->port_line->text().toInt();
    QHostAddress serverAddress = QHostAddress(serverIpAddress);

    ushort status = _server_manager->toggleServer(serverAddress, port);
    if (status == 1)
    {
        ui->btn_listen->setText("Stop Listening");
    }
    else if (status == 0)
    {
        ui->btn_listen->setText("Start Listening");
    }
    else
    {
        QMessageBox::warning(this, "Error", "TCP Server Listening Error");
    }
}

void MainWindow::setClientName(QString name)
{
    auto widget = qobject_cast<QWidget*>(sender());
    auto index = ui->message_tabs->indexOf(widget);
    ui->message_tabs->setTabText(index, name);
}

void MainWindow::setClientStatus(MessageProtocol::Status status)
{
    auto widget = qobject_cast<QWidget*>(sender());
    auto index = ui->message_tabs->indexOf(widget);
    QString iconName = ":/icons/";
    switch (status)
    {
    case MessageProtocol::Available:
        iconName.append("available.png");
        break;
    case MessageProtocol::Away:
        iconName.append("away.png");
        break;
    case MessageProtocol::Busy:
        iconName.append("busy.png");
        break;
    }

    auto icon = QIcon(iconName);
    ui->message_tabs->setTabIcon(index, icon);
}
