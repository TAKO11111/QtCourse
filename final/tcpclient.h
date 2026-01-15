#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QMutex>
#include "tcpmessage.h"

// TCP客户端类：处理连接、消息收发、心跳、重连（运行在子线程）
class TcpClient : public QObject {
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient();

    // 连接服务器
    void connectToServer(const QString& ip, quint16 port);
    // 断开连接
    void disconnectFromServer();
    // 发送消息（封装为TcpMessage对象）
    void sendMessage(const TcpMessage& msg);
    // 设置心跳间隔（默认30秒）
    void setHeartbeatInterval(int intervalMs = 30000);
    // 获取当前连接状态
    bool isConnected() const { return m_isConnected; }

signals:
    // 连接状态变化（true=已连接，false=已断开）
    void connectStateChanged(bool connected);
    // 收到消息（解析后的TcpMessage对象）
    void messageReceived(const TcpMessage& msg);
    // 错误信息
    void errorOccurred(const QString& errorMsg);

private slots:
    // 内部槽函数：处理连接成功
    void onConnected();
    // 内部槽函数：处理断开连接
    void onDisconnected();
    // 内部槽函数：处理收到数据
    void onReadyRead();
    // 内部槽函数：处理socket错误
    void onSocketError(QAbstractSocket::SocketError error);
    // 内部槽函数：发送心跳包
    void sendHeartbeat();
    // 内部槽函数：重连服务器
    void reconnect();

private:
    QTcpSocket* m_socket = nullptr;       // TCP套接字
    QTimer* m_heartbeatTimer = nullptr;   // 心跳定时器
    QTimer* m_reconnectTimer = nullptr;   // 重连定时器
    int m_heartbeatInterval = 30000;      // 心跳间隔（毫秒）
    QString m_serverIp;                   // 服务器IP
    quint16 m_serverPort = 0;             // 服务器端口
    bool m_isConnected = false;           // 连接状态
    QMutex m_mutex;                       // 线程安全锁
};

// 线程管理类：单例模式，将TcpClient移到子线程运行
class TcpThreadManager {
public:
    // 获取单例实例
    static TcpThreadManager& getInstance() {
        static TcpThreadManager instance;
        return instance;
    }

    // 获取TcpClient对象（用于外部调用）
    TcpClient* getTcpClient() { return m_tcpClient; }

private:
    TcpThreadManager();
    ~TcpThreadManager();

    // 禁止拷贝和赋值
    TcpThreadManager(const TcpThreadManager&) = delete;
    TcpThreadManager& operator=(const TcpThreadManager&) = delete;

    QThread* m_thread = nullptr;          // 网络线程
    TcpClient* m_tcpClient = nullptr;     // TCP客户端实例
};

#endif // TCPCLIENT_H
