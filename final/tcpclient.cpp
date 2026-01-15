#include "tcpclient.h"
#include <QJsonDocument>
#include <QDebug>
#include <QThread>
#include <QCoreApplication>

TcpClient::TcpClient(QObject *parent) : QObject(parent),
    m_heartbeatInterval(30000),
    m_isInitialized(false)
{
    qDebug() << "TcpClient 构造函数 - 当前线程:" << QThread::currentThread();

    // 必须确保对象在同一个线程中创建所有子对象
    m_socket = new QTcpSocket(this);
    m_heartbeatTimer = new QTimer(this);

    // 连接信号槽 - 使用DirectConnection确保在正确线程
    connect(m_socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead, Qt::DirectConnection);
    connect(m_socket, &QTcpSocket::stateChanged, this, &TcpClient::onSocketStateChanged, Qt::DirectConnection);
    connect(m_heartbeatTimer, &QTimer::timeout, this, &TcpClient::onHeartbeatTimeout, Qt::DirectConnection);

    m_isInitialized = true;
    qDebug() << "TcpClient 初始化完成";
}

TcpClient::~TcpClient() {
    qDebug() << "TcpClient 析构函数 - 当前线程:" << QThread::currentThread();
    disconnectFromServer();
}

bool TcpClient::connectToServer(const QString& ip, quint16 port) {
    qDebug() << "TcpClient::connectToServer - 当前线程:" << QThread::currentThread()
        << "，目标:" << ip << ":" << port
        << "，对象线程:" << this->thread()
        << "，Socket线程:" << m_socket->thread();

    // 确保在正确线程中操作
    if (QThread::currentThread() != this->thread()) {
        qDebug() << "警告：connectToServer 在错误线程调用！";
    }

    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        qDebug() << "已连接到服务器，无需重复连接";
        return true;
    }

    // 如果正在连接，先断开
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->abort();
    }

    qDebug() << "开始连接服务器...";
    m_socket->connectToHost(ip, port);

    bool connected = m_socket->waitForConnected(5000);

    if (connected) {
        qDebug() << "连接成功！";

        // 确保定时器在当前线程启动
        if (!m_heartbeatTimer->isActive()) {
            m_heartbeatTimer->start(m_heartbeatInterval);
            qDebug() << "心跳定时器已启动，间隔:" << m_heartbeatInterval << "ms";
        }
    } else {
        qDebug() << "连接失败:" << m_socket->errorString();
    }

    return connected;
}

void TcpClient::disconnectFromServer() {
    qDebug() << "TcpClient::disconnectFromServer - 当前线程:" << QThread::currentThread();

    // 停止心跳定时器
    if (m_heartbeatTimer && m_heartbeatTimer->isActive()) {
        m_heartbeatTimer->stop();
        qDebug() << "心跳定时器已停止";
    }

    // 断开socket连接
    if (m_socket) {
        if (m_socket->state() != QAbstractSocket::UnconnectedState) {
            m_socket->disconnectFromHost();
            if (m_socket->state() != QAbstractSocket::UnconnectedState) {
                m_socket->waitForDisconnected(1000);
            }
        }
    }
}

qint64 TcpClient::sendMessage(const TcpMessage& msg) {
    qDebug() << "TcpClient::sendMessage - 当前线程:" << QThread::currentThread();

    if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "发送失败：未连接到服务器";
        emit errorOccurred("未连接到服务器");
        return -1;
    }

    QJsonObject obj = msg.toJson();
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    qint64 bytesWritten = m_socket->write(data + "\n");

    if (bytesWritten > 0) {
        m_socket->flush();
        qDebug() << "消息发送成功，大小:" << bytesWritten;
    }

    return bytesWritten;
}

void TcpClient::setHeartbeatInterval(int interval) {
    qDebug() << "TcpClient::setHeartbeatInterval - 当前线程:" << QThread::currentThread()
        << "，新间隔:" << interval;

    m_heartbeatInterval = interval;
    if (m_heartbeatTimer->isActive()) {
        m_heartbeatTimer->setInterval(interval);
    }
}

void TcpClient::debugThreadInfo() {
    qDebug() << "=== TcpClient 线程信息 ===";
    qDebug() << "当前线程:" << QThread::currentThread();
    qDebug() << "对象线程:" << this->thread();
    if (m_socket) {
        qDebug() << "Socket线程:" << m_socket->thread();
        qDebug() << "Socket状态:" << m_socket->state();
    }
    if (m_heartbeatTimer) {
        qDebug() << "定时器线程:" << m_heartbeatTimer->thread();
        qDebug() << "定时器状态:" << (m_heartbeatTimer->isActive() ? "激活" : "未激活");
    }
    qDebug() << "=========================";
}

void TcpClient::onReadyRead() {
    while (m_socket->canReadLine()) {
        QByteArray line = m_socket->readLine().trimmed();
        if (line.isEmpty()) continue;

        QJsonDocument doc = QJsonDocument::fromJson(line);
        if (!doc.isObject()) continue;

        TcpMessage msg = TcpMessage::fromJson(doc.object());
        emit messageReceived(msg);
    }
}

void TcpClient::onSocketStateChanged(QAbstractSocket::SocketState state) {
    bool connected = (state == QAbstractSocket::ConnectedState);

    // 只发送最终连接状态，避免中间状态干扰
    static QAbstractSocket::SocketState lastState = QAbstractSocket::UnconnectedState;

    if (state != lastState) {
        qDebug() << "Socket状态变化:" << lastState << "->" << state;
        lastState = state;

        // 只有在真正连接或断开时才发送信号
        if (state == QAbstractSocket::ConnectedState ||
            state == QAbstractSocket::UnconnectedState) {
            emit connectStateChanged(connected);
        }
    }
}

void TcpClient::onHeartbeatTimeout() {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        TcpMessage msg(MessageType::Heartbeat, "", "", "PING");
        sendMessage(msg);
    }
}
