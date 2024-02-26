#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 创建UDP对象
    socket = new QUdpSocket(this);

    // 将状态栏添加到下方
    stateLabel = new QLabel("Socket状态: ");
    ui->statusbar->addWidget(stateLabel);

    // 设置WindowsTitle
    QString localIP = getLocalIP();
    setWindowTitle(windowTitle() + " -- 本机: " + localIP);
    ui->comboBox->addItem(localIP);

    // 连接槽函数
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getLocalIP()
{
    QString hostName = QHostInfo::localHostName();
    QHostInfo hostInfo = QHostInfo::fromName(hostName);

    // 定义本机IP
    QString localIP = "";
    QList<QHostAddress> addressList = hostInfo.addresses();

    // 判断列表是否为空
    if (addressList.isEmpty()){
        return localIP;
    }

    for (int i = 0; i < addressList.count(); i++) {
        QHostAddress address = addressList.at(i);
        if (address.protocol() == QAbstractSocket::IPv4Protocol){
            qDebug() << address.toString();
            localIP = address.toString();
            break;
        }
    }
    return localIP;
}

void MainWindow::onStateChanged(QAbstractSocket::SocketState socketState)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    stateLabel->setText("Sockect当前状态: " + QString(metaEnum.valueToKey(socketState)));
}

// 绑定槽
void MainWindow::on_actionBind_triggered()
{
    QString targetIP = ui->comboBox->currentText();
    QHostAddress targetAddr(targetIP);
    quint16 targetPort = ui->spinBox->value();

    // 绑定成功
    if(socket->bind(targetAddr, targetPort)){
        ui->actionBind->setEnabled(false);
        ui->actionStop->setEnabled(true);
        ui->spinBox->setEnabled(false);
        ui->comboBox->setEnabled(false);
        ui->plainTextEdit->appendPlainText("**绑定成功 - IP:" + targetIP + " 端口:" + QString::number(targetPort));
    }
    else{
        ui->plainTextEdit->appendPlainText("**绑定失败");
    }
}

// 停止绑定
void MainWindow::on_actionStop_triggered()
{
    socket->abort();
    ui->actionBind->setEnabled(true);
    ui->actionStop->setEnabled(false);
    ui->spinBox->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->plainTextEdit->appendPlainText("**解除绑定");
}

// 发送消息
void MainWindow::on_pushButton_clicked()
{
    QString message = ui->lineEdit->text();
    if (message == ""){
        return;
    }
    QHostAddress localHost(ui->comboBox->currentText());
    quint16 port = ui->spinBox_2->value();
    int res = socket->writeDatagram(message.toUtf8(), localHost, port);
    if (res == -1){
        ui->plainTextEdit->appendPlainText("[数据发送失败]");
    }
    else{
        ui->plainTextEdit->appendPlainText("[发送] " + message);
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
    }
}

// 有消息需要读取时处理
void MainWindow::onReadyRead()
{
    while(socket->hasPendingDatagrams()){
        // 定义三个空的数据用来接收数据
        QHostAddress peerAddr;
        quint16 peerPort;
        QByteArray receiveMsg;
        receiveMsg.resize(socket->pendingDatagramSize());
        socket->readDatagram(receiveMsg.data(), receiveMsg.size(), &peerAddr, &peerPort); // 读到数据后展示到plaint中
        if (QString(receiveMsg) != "")
        {
         ui->plainTextEdit->appendPlainText("From " + peerAddr.toString() + ": " + QString(receiveMsg));
        }

    }
}
