#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    is_connected = false;
    // disable input and status until connection established
    ui->input_widget->setEnabled(false);
    ui->status_combo_box->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::messageReceived(QString message)
{
    ui->message_list->append(QString("<font size='1'>%1</font>").arg(QDateTime::currentDateTime().toString("MM/dd/yyyy (ddd)  hh:mm:ss")));
    ui->message_list->append(message);
}

void MainWindow::on_btn_send_clicked()
{
    auto message = ui->message_text->text().trimmed();
    _client_manager->sendMessage(message);
    ui->message_list->append(QString("<font size='1'>%1</font>").arg(QDateTime::currentDateTime().toString("MM/dd/yyyy (ddd)  hh:mm:ss")));
    ui->message_list->append(message);
    ui->message_text->setText("");
}


void MainWindow::on_btn_connect_clicked()
{
    if (!is_connected)
    {
        QHostAddress ip = QHostAddress(ui->server_ip_line->text());
        ushort port = ui->port_line->text().trimmed().toInt();
        QString username = ui->name_line->text().trimmed();
        _client_manager = new ClientManager(ip, port);

        connect(_client_manager, &ClientManager::connected, this, [this]() {
            ui->input_widget->setEnabled(true);
        });
        connect(_client_manager, &ClientManager::disconnected, this, [this]() {
            ui->input_widget->setEnabled(false);
        });
        connect(_client_manager, &ClientManager::connected, this, [this]() {
            ui->connection_specs->setEnabled(false);
        });
        connect(_client_manager, &ClientManager::disconnected, this, [this]() {
            ui->connection_specs->setEnabled(true);
        });
        connect(_client_manager, &ClientManager::connected, this, [this]() {
            ui->status_combo_box->setEnabled(true);
        });
        connect(_client_manager, &ClientManager::disconnected, this, [this]() {
            ui->status_combo_box->setEnabled(false);
        });
        connect(_client_manager, &ClientManager::messageReceived, this, &MainWindow::messageReceived);
        connect(_client_manager, &ClientManager::otherSideisTyping, this, &MainWindow::onTyping);
        connect(ui->message_text, &QLineEdit::textChanged, _client_manager, &ClientManager::sendIsTyping);

        _client_manager->connectToServer();
        ui->message_list->append(QString("<font color='green' size='1'>%1</font>").arg("CONNECTION ESTABLISHED"));
        ui->btn_connect->setText("Disconnect");
        if (username != "")
        {
            _client_manager->sendName(username);
        }
    }
    else
    {
        _client_manager->disconnectFromServer();
        ui->message_list->append(QString("<font color='red' size='1'>%1</font>").arg("CONNECTION TERMINATED"));
        delete _client_manager;
        _client_manager = nullptr;
        ui->btn_connect->setText("connect");
    }
    is_connected = !is_connected;
}

void MainWindow::on_status_combo_box_currentIndexChanged(int index)
{
    auto status = (MessageProtocol::Status) index;
    _client_manager->sendStatus(status);
}

void MainWindow::onTyping()
{
    statusBar()->showMessage("Server is typing...", 750);
}
