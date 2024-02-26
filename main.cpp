#include "mainwindow.h"

#include <QApplication>

// 1. UDP(User Datagram Protocol, 用户数据报协议) 是轻量的、不可靠的、面向数据报(datagram)、无连接的协议、
// 它可以用于对可靠性要求不高的场合
// 两个程序之间进行UDP通信无需预先建立持久的socket连接，UDP每次发送数据报都需要指定目标地址和端口
// 2. UDP消息传送方式有单播， 广播， 组播(多播)三种模式
// 3. UDPSocket 是收发一体的

// 目标: 使用UDP实现单播

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
