#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QInputDialog>
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
    void messageReceived(QString message);
    void on_btn_send_clicked();
    void on_btn_connect_clicked();
    void on_status_combo_box_currentIndexChanged(int index);
    void onTyping();

private:
    Ui::MainWindow* ui;
    ClientManager* _client_manager;
    bool is_connected;
};
#endif // MAINWINDOW_H
