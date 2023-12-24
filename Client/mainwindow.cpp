#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // one client window corresponds to a client manager
    _client_manager = new ClientManager(this);

    // connect the signals and slots for the client manager
    connect(_client_manager, &ClientManager::connected, this, [this]() {
        ui->input_widget->setEnabled(true);
        ui->connection_specs->setEnabled(false);
        ui->status_combo_box->setEnabled(true);
    });
    connect(_client_manager, &ClientManager::disconnected, this, [this]() {
        ui->input_widget->setEnabled(false);
        ui->connection_specs->setEnabled(true);
        ui->status_combo_box->setEnabled(false);
    });

    connect(_client_manager, &ClientManager::connected, this, &MainWindow::handleConnected);
    connect(_client_manager, &ClientManager::disconnected, this, &MainWindow::handleDisconnected);

    connect(_client_manager, &ClientManager::messageReceived, this, &MainWindow::messageReceived);
    connect(_client_manager, &ClientManager::otherSideisTyping, this, &MainWindow::onTyping);
    connect(ui->message_text, &QLineEdit::textChanged, _client_manager, &ClientManager::sendIsTyping);

    connect(_client_manager, &ClientManager::fileRequestReceived, this, &MainWindow::handleFileRequest);
    connect(_client_manager, &ClientManager::fileRejected, this, &MainWindow::handleFileRejection);

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
    QString timestamp = QDateTime::currentDateTime().toString("MM/dd/yyyy (ddd)  hh:mm:ss");
    QString msg = QString("<font size='1'>%1</font><br>%2<br>").arg(timestamp, message);
    ui->message_list->insertHtml(msg);
}

void MainWindow::on_btn_send_clicked()
{
    auto message = QString(ui->message_text->text().trimmed());
    if (message != "")
    {
        _client_manager->sendMessage(message);
        QString timestamp = QDateTime::currentDateTime().toString("MM/dd/yyyy (ddd)  hh:mm:ss");
        QString msg = QString("<font size='1'>%1</font><br>%2<br>").arg(timestamp, message);
        ui->message_list->insertHtml(msg);
        ui->message_text->setText("");
    }
}


void MainWindow::on_btn_connect_clicked()
{
    if (!_client_manager->isConnected())
    {
        _client_manager->setIp(QHostAddress(ui->server_ip_line->text()));
        _client_manager->setPort(ui->port_line->text().trimmed().toInt());

        _client_manager->connectToServer();
    }
    else
    {
        _client_manager->disconnectFromServer();
    }
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

void MainWindow::handleFileRequest(QString username, QString filename, qint64 filesize)
{
    auto message = QString("%1 is trying to upload a file. Do you want to accept it?\nFile name: %2\nFile size: %3 Bytes").arg(username, filename).arg(filesize);
    auto decision = QMessageBox::question(this, "Incoming File", message);
    if (decision == QMessageBox::Yes)
    {
        _client_manager->sendAcceptUpload();
    }
    else
    {
        _client_manager->sendRejectUpload();
    }
}

void MainWindow::handleFileRejection()
{
    QMessageBox::critical(this, "Unsuccessful", "The server declined your file upload request.");
}

void MainWindow::handleConnected()
{
    QString username = ui->name_line->text().trimmed();
    ui->message_list->insertHtml(QString("<font color='green' size='1.5'><strong>%1</strong></font><br>").arg("CONNECTION ESTABLISHED"));
    ui->btn_connect->setText("Disconnect");
    if (username != "")
    {
        _client_manager->sendName(username);
    }
}

void MainWindow::handleDisconnected()
{
    ui->message_list->insertHtml(QString("<font color='red' size='1.5'><strong>%1</strong></font>").arg("CONNECTION TERMINATED"));
    ui->btn_connect->setText("connect");
}

void MainWindow::on_btn_upload_clicked()
{
    auto filename = QFileDialog::getOpenFileName(this, "Select a file", "/home");
    _client_manager->sendRequestUpload(filename);
}

