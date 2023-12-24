#include "clientchatwidget.h"
#include "ui_clientchatwidget.h"

ClientChatWidget::ClientChatWidget(QTcpSocket* client, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ClientChatWidget)
{
    ui->setupUi(this);
    _client = new ClientManager(client, this);
    ui->message_list->append(QString("<font color='green' size='1'><em><strong>%1</strong></em></font>").arg("CONNECTION ESTABLISHED"));
    connect(_client, &ClientManager::disconnected, this, &ClientChatWidget::clientDisconnected);
    connect(_client, &ClientManager::messageReceived, this, &ClientChatWidget::messageReceived);
    connect(_client, &ClientManager::otherSideisTyping, this, &ClientChatWidget::onTyping);
    connect(_client, &ClientManager::nameChanged, this, &ClientChatWidget::nameChanged);
    connect(_client, &ClientManager::statusChanged, this, &ClientChatWidget::statusChanged);
    connect(_client, &ClientManager::fileRequestReceived, this, &ClientChatWidget::handleFileRequest);
    connect(_client, &ClientManager::fileSaved, this, &ClientChatWidget::onFileSaved);
    connect(ui->message_text, &QLineEdit::textChanged, _client, &ClientManager::sendIsTyping);
}

ClientChatWidget::~ClientChatWidget()
{
    delete ui;
}

void ClientChatWidget::on_btn_send_clicked()
{
    // messages sent by the server are coloured blue
    auto message = QString("<font color='blue'>%1</font>").arg(ui->message_text->text().trimmed());
    _client->sendMessage(message);
    ui->message_list->append(QString("<font size='1'>%1</font>").arg(QDateTime::currentDateTime().toString("MM/dd/yyyy (ddd)  hh:mm:ss")));
    ui->message_list->append(message);
    ui->message_list->append("");
    ui->message_text->setText("");
}

void ClientChatWidget::clientDisconnected()
{
    ui->message_list->append(QString("<font color='red' size='1'><em><strong>%1</strong></em></font>").arg("CONNECTION TERMINATED"));
    ui->widget_send->setEnabled(false);
}

void ClientChatWidget::messageReceived(QString message)
{
    ui->message_list->append(QString("<font size='1'>%1</font>").arg(QDateTime::currentDateTime().toString("MM/dd/yyyy (ddd)  hh:mm:ss")));
    ui->message_list->append(message);
    ui->message_list->append("");
}

void ClientChatWidget::nameChanged(QString name)
{
    emit clientNameChanged(name);
}

void ClientChatWidget::statusChanged(MessageProtocol::Status status)
{
    emit clientStatusChanged(status);
}

void ClientChatWidget::onTyping()
{
    emit clientIsTyping(_client->username());
}

void ClientChatWidget::handleFileRequest(QString username, QString filename, qint64 filesize)
{
    auto message = QString("%1 is trying to upload a file. Accept?\nFile name: %2\nFile size: %3 Bytes").arg(username, filename).arg(filesize);
    auto decision = QMessageBox::question(this, "Incoming File", message);
    if (decision == QMessageBox::Yes)
    {
        _client->sendAcceptUpload();
    }
    else
    {
        _client->sendRejectUpload();
    }
}

void ClientChatWidget::onFileSaved(QString path)
{
    auto message = QString("File saved at:\n%1").arg(path);
    QMessageBox::information(this, "Successful", message);
}
