#include "tcpclient.h"
#include <QThread>
#include <QDebug>

// ========== TcpClient实现 ==========
TcpClient::TcpClient(QObject *parent) : QObject(parent) {
    // 初始化socket和定时器
    m_socket = new QTcpSocket(this);
    m_heartbeatTimer = new QTimer(this);
    m_reconnectTimer = new QTimer(this);

    // 设置重连间隔（5秒）
    m_reconnectTimer->setInterval(5000);

    // 绑定socket信号槽
    connect(m_socket, &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &TcpClient::onSocketError);

    // 绑定定时器信号槽
    connect(m_heartbeatTimer, &QTimer::timeout, this, &TcpClient::sendHeartbeat);
    connect(m_reconnectTimer, &QTimer::timeout, this, &TcpClient::reconnect);
}

TcpClient::~TcpClient() {
    disconnectFromServer();
}

void TcpClient::connectToServer(const QString& ip, quint16 port) {
    QMutexLocker locker(&m_mutex);
    m_serverIp = ip;
    m_serverPort = port;

    if (m_socket->state() != QTcpSocket::UnconnectedState) {
        m_socket->disconnectFromHost();
    }

    qDebug() << "[网络] 尝试连接服务器：" << ip << ":" << port;
    m_socket->connectToHost(ip, port);
}

void TcpClient::disconnectFromServer() {
    QMutexLocker locker(&m_mutex);
    m_heartbeatTimer->stop();
    m_reconnectTimer->stop();

    if (m_socket->state() != QTcpSocket::UnconnectedState) {
        m_socket->disconnectFromHost();
    }

    m_isConnected = false;
    emit connectStateChanged(false);
    qDebug() << "[网络] 主动断开服务器连接";
}

void TcpClient::sendMessage(const TcpMessage& msg) {
    QMutexLocker locker(&m_mutex);
    if (!m_isConnected) {
        emit errorOccurred("[网络错误] 未连接到服务器，无法发送消息");
        return;
    }

    // 将消息转为JSON字符串，再转为字节数组发送
    QString jsonStr = msg.toJsonString();
    QByteArray data = jsonStr.toUtf8();

    // 发送数据（实际项目可添加消息长度前缀，防止粘包）
    qint64 sendLen = m_socket->write(data);
    if (sendLen == -1) {
        emit errorOccurred("[网络错误] 消息发送失败：" + m_socket->errorString());
    } else {
        qDebug() << "[网络] 发送消息成功：" << jsonStr;
    }
}

void TcpClient::setHeartbeatInterval(int intervalMs) {
    m_heartbeatInterval = intervalMs;
    m_heartbeatTimer->setInterval(intervalMs);
}

void TcpClient::onConnected() {
    QMutexLocker locker(&m_mutex);
    m_isConnected = true;
    m_reconnectTimer->stop(); // 停止重连
    m_heartbeatTimer->start(m_heartbeatInterval); // 启动心跳

    emit connectStateChanged(true);
    qDebug() << "[网络] 连接服务器成功";
}

void TcpClient::onDisconnected() {
    QMutexLocker locker(&m_mutex);
    m_isConnected = false;
    m_heartbeatTimer->stop(); // 停止心跳
    m_reconnectTimer->start(); // 启动重连

    emit connectStateChanged(false);
    qDebug() << "[网络] 与服务器断开连接，将自动重连";
}

void TcpClient::onReadyRead() {
    // 读取所有收到的数据
    QByteArray data = m_socket->readAll();
    QString jsonStr = QString::fromUtf8(data);
    qDebug() << "[网络] 收到消息：" << jsonStr;

    // 解析为TcpMessage对象并发送信号
    TcpMessage msg = TcpMessage::fromJsonString(jsonStr);
    emit messageReceived(msg);
}

void TcpClient::onSocketError(QAbstractSocket::SocketError error) {
    if (error == QAbstractSocket::RemoteHostClosedError) {
        return; // 远程主机关闭连接，已在onDisconnected处理
    }

    QString errorMsg = "[网络错误] " + m_socket->errorString();
    qDebug() << errorMsg;
    emit errorOccurred(errorMsg);
}

void TcpClient::sendHeartbeat() {
    // 构造心跳消息
    TcpMessage heartbeatMsg(MessageType::Heartbeat, "", "", "PING");
    sendMessage(heartbeatMsg);
    qDebug() << "[网络] 发送心跳包";
}

void TcpClient::reconnect() {
    QMutexLocker locker(&m_mutex);
    if (!m_isConnected && !m_serverIp.isEmpty() && m_serverPort > 0) {
        qDebug() << "[网络] 尝试重连服务器...";
        m_socket->connectToHost(m_serverIp, m_serverPort);
    }
}

// ========== TcpThreadManager实现 ==========
TcpThreadManager::TcpThreadManager() {
    // 创建子线程
    m_thread = new QThread();
    // 创建TcpClient实例
    m_tcpClient = new TcpClient();
    // 将TcpClient移到子线程（避免阻塞UI）
    m_tcpClient->moveToThread(m_thread);
    // 启动线程
    m_thread->start();
}

TcpThreadManager::~TcpThreadManager() {
    // 停止线程
    m_thread->quit();
    m_thread->wait();
    // 释放资源
    delete m_tcpClient;
    delete m_thread;
}
