#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QFileDialog>
#include "clientmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleConnected();
    void handleDisconnected();

    void messageReceived(QString message);
    void on_btn_send_clicked();
    void on_btn_connect_clicked();
    void on_status_combo_box_currentIndexChanged(int index);
    void onTyping();

    void handleFileRequest(QString username, QString filename, qint64 filesize);
    void handleFileRejection();

    void on_btn_upload_clicked();

private:
    Ui::MainWindow* ui;
    ClientManager*  _client_manager;
};
#endif // MAINWINDOW_H
