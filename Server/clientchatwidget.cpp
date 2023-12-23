#include "clientchatwidget.h"
#include "ui_clientchatwidget.h"

ClientChatWidget::ClientChatWidget(QTcpSocket* client, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ClientChatWidget)
{
    ui->setupUi(this);
    _client = new ClientManager(client, this);
    ui->message_list->append(QString("<font color='green' size='1'>%1</font>").arg("CONNECTION ESTABLISHED"));
    connect(_client, &ClientManager::disconnected, this, &ClientChatWidget::clientDisconnected);
    connect(_client, &ClientManager::messageReceived, this, &ClientChatWidget::messageReceived);
    connect(_client, &ClientManager::otherSideisTyping, this, &ClientChatWidget::onTyping);
    connect(_client, &ClientManager::nameChanged, this, &ClientChatWidget::nameChanged);
    connect(_client, &ClientManager::statusChanged, this, &ClientChatWidget::statusChanged);
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
    ui->message_text->setText("");
}

void ClientChatWidget::clientDisconnected()
{
    ui->message_list->append(QString("<font color='red' size='1'>%1</font>").arg("CONNECTION TERMINATED"));
    ui->widget_send->setEnabled(false);
}

void ClientChatWidget::messageReceived(QString message)
{
    ui->message_list->append(QString("<font size='1'>%1</font>").arg(QDateTime::currentDateTime().toString("MM/dd/yyyy (ddd)  hh:mm:ss")));
    ui->message_list->append(message);
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
    emit clientIsTyping(_client->name());
}
