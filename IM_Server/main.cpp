#include <QTcpServer>
#include <QTcpSocket>
#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 直接创建QTcpServer对象（无拷贝操作）
    QTcpServer server;
    if (!server.listen(QHostAddress::Any, 8888)) {
        qDebug() << "Server start failed:" << server.errorString();
        return -1;
    }
    qDebug() << "Test server started on port 8888";

    // 关联信号时，捕获server的引用（而非拷贝）
    QObject::connect(&server, &QTcpServer::newConnection, [&server]() {
        QTcpSocket* client = server.nextPendingConnection();
        if (!client) return;

        qDebug() << "Client connected:" << client->peerAddress().toString();

        // 转发消息逻辑
        QObject::connect(client, &QTcpSocket::readyRead, [client, &server]() {
            QByteArray data = client->readAll();
            for (QObject* obj : server.children()) {
                QTcpSocket* sock = qobject_cast<QTcpSocket*>(obj);
                if (sock && sock != client && sock->state() == QAbstractSocket::ConnectedState) {
                    sock->write(data);
                }
            }
        });

        // 客户端断开逻辑
        QObject::connect(client, &QTcpSocket::disconnected, [client]() {
            qDebug() << "Client disconnected:" << client->peerAddress().toString();
            client->deleteLater();
        });
    });

    return a.exec();
}
