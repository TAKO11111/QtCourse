#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "tcpmessage.h"

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient() override;

    // 线程安全方法声明
    Q_INVOKABLE bool connectToServer(const QString& ip, quint16 port);
    Q_INVOKABLE void disconnectFromServer();
    Q_INVOKABLE qint64 sendMessage(const TcpMessage& msg);
    Q_INVOKABLE void setHeartbeatInterval(int interval);
    Q_INVOKABLE void debugThreadInfo();

signals:
    void messageReceived(const TcpMessage& msg);
    void connectStateChanged(bool connected);
    void errorOccurred(const QString& errorMsg);

private slots:
    void onReadyRead();
    void onSocketStateChanged(QAbstractSocket::SocketState state);
    void onHeartbeatTimeout();

private:
    QTcpSocket* m_socket;
    QTimer* m_heartbeatTimer;
    int m_heartbeatInterval;
    bool m_isInitialized;
};

#endif // TCPCLIENT_H
