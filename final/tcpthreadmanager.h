#ifndef TCPTHREADMANAGER_H
#define TCPTHREADMANAGER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "tcpclient.h"

class TcpThreadManager : public QObject
{
    Q_OBJECT
public:
    static TcpThreadManager& getInstance();
    TcpClient* getTcpClient() const { return m_tcpClient; }

    // 线程安全的公共接口
    bool connectToServer(const QString& ip, quint16 port);
    qint64 sendMessage(const TcpMessage& msg);
    void setHeartbeatInterval(int interval);

    // 调试方法
    void debugThreadInfo();

signals:
    void messageReceived(const TcpMessage& msg);
    void connectStateChanged(bool connected);
    void errorOccurred(const QString& errorMsg);

private:
    explicit TcpThreadManager(QObject *parent = nullptr);
    ~TcpThreadManager() override;
    TcpThreadManager(const TcpThreadManager&) = delete;
    TcpThreadManager& operator=(const TcpThreadManager&) = delete;

    void initClient();

    QThread* m_thread;
    TcpClient* m_tcpClient;
    QMutex m_mutex;
    bool m_initialized;
};

#endif // TCPTHREADMANAGER_H
