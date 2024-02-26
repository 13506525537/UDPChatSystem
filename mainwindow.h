#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QUdpSocket>
#include <QLabel>
#include <QHostInfo>
#include <QMetaEnum>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 获取本机IP
    QString getLocalIP();

public slots:
    // 准备好读取的槽函数
    void onReadyRead();

    // 状态改变
    void onStateChanged(QAbstractSocket::SocketState socketState);

private slots:
    void on_actionBind_triggered();

    void on_actionStop_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    // 构建状态栏
    QLabel * stateLabel;

    // 创建socket对象
    QUdpSocket * socket;
};

#endif // MAINWINDOW_H
