#ifndef MESSAGEPROTOCOL_H
#define MESSAGEPROTOCOL_H

#include <QByteArray>
#include <QString>
#include <QIODevice>
#include <QFileInfo>

class MessageProtocol
{
public:
    enum MessageType
    {
        Text,
        IsTyping,
        SetName,
        SetStatus,
        RequestUpload,
        AcceptUpload,
        RejectUpload,
        Upload
    };

    enum Status
    {
        Available,
        Away,
        Busy
    };

    MessageProtocol();

    // message
    QByteArray textMessage(QString message);
    QByteArray isTypingMessage();
    // name and status
    QByteArray setNameMessage(QString name);
    QByteArray setStatusMessage(Status status);
    // file
    QByteArray setRequestUploadMessage(QString filename);
    QByteArray setAcceptUploadMessage();
    QByteArray setRejectUploadMessage();
    QByteArray setUploadMessage(QString filename);

    void parseData(QByteArray data);

    QString message() const;
    QString username() const;
    Status status() const;
    MessageType type() const;

    QString filename() const;

    qint64 filesize() const;

    QByteArray filedata() const;

private:
    MessageType _type;
    QString     _message;
    QString     _username;
    Status      _status;
    QString     _filename;
    qint64      _filesize;
    QByteArray  _filedata;

    QByteArray convertData(MessageType type, QString data);
};

#endif // MESSAGEPROTOCOL_H
