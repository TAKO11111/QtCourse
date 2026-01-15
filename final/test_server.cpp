#include "tcpmessage.h"
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QMap>

QMap<QTcpSocket*, QString> g_clientMap;

void onNewConnection(QTcpServer* server) {
    QTcpSocket* socket = server->nextPendingConnection();
    qDebug() << "New client connected:" << socket->peerAddress().toString();

    QObject::connect(socket, &QTcpSocket::readyRead, [=]() {
        while (socket->canReadLine()) {
            QByteArray line = socket->readLine().trimmed();
            if (line.isEmpty()) continue;

            QJsonDocument doc = QJsonDocument::fromJson(line);
            if (!doc.isObject()) continue;

            QJsonObject obj = doc.object();
            int type = obj["type"].toInt();

            if (type == static_cast<int>(MessageType::Heartbeat)) {
                obj["content"] = "PONG";
                socket->write(QJsonDocument(obj).toJson(QJsonDocument::Compact) + "\n");
                continue;
            }

            if (type == static_cast<int>(MessageType::SingleChat)) {
                QString from = obj["from"].toString();
                QString to = obj["to"].toString();
                qDebug() << "Chat message:" << from << "->" << to << ":" << obj["content"].toString();

                if (g_clientMap[socket].isEmpty()) {
                    g_clientMap[socket] = from;
                }

                for (auto client : g_clientMap.keys()) {
                    if (client != socket && g_clientMap[client] == to) {
                        client->write(line + "\n");
                        break;
                    }
                }
            }
        }
    });

    QObject::connect(socket, &QTcpSocket::disconnected, [=]() {
        qDebug() << "Client disconnected:" << socket->peerAddress().toString();
        g_clientMap.remove(socket);
        socket->deleteLater();
    });

    QObject::connect(socket, &QTcpSocket::errorOccurred, [=](QAbstractSocket::SocketError error) {
        qDebug() << "Client error:" << socket->errorString();
        g_clientMap.remove(socket);
        socket->deleteLater();
    });
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTcpServer server;
    if (!server.listen(QHostAddress::Any, 8888)) {
        qCritical() << "Server start failed:" << server.errorString();
        return -1;
    }

    qDebug() << "Test server started on port 8888";

    QObject::connect(&server, &QTcpServer::newConnection, [&]() {
        onNewConnection(&server);
    });

    return a.exec();
}
