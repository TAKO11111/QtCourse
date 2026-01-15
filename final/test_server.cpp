#include <QTcpServer>
#include <QTcpSocket>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

class TestServer : public QTcpServer {
    Q_OBJECT
public:
    TestServer(QObject *parent = nullptr) : QTcpServer(parent) {
        // 监听本地8888端口
        if (!listen(QHostAddress::LocalHost, 8888)) {
            qDebug() << "[服务器] 启动失败：" << errorString();
        } else {
            qDebug() << "[服务器] 启动成功，监听127.0.0.1:8888";
        }
    }

protected:
    void incomingConnection(qintptr socketDescriptor) override {
        QTcpSocket* clientSocket = new QTcpSocket(this);
        clientSocket->setSocketDescriptor(socketDescriptor);
        qDebug() << "[服务器] 客户端连接：" << clientSocket->peerAddress().toString();

        // 接收客户端消息并回显（模拟转发）
        connect(clientSocket, &QTcpSocket::readyRead, this, [=]() {
            QByteArray data = clientSocket->readAll();
            QString jsonStr = QString::fromUtf8(data);
            qDebug() << "[服务器] 收到消息：" << jsonStr;

            // 解析消息并原路返回（模拟转发给目标用户）
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (doc.isObject()) {
                QJsonObject obj = doc.object();
                // 交换from/to，模拟对方回复
                QString from = obj["from"].toString();
                obj["from"] = obj["to"].toString();
                obj["to"] = from;

                QByteArray sendData = QJsonDocument(obj).toJson(QJsonDocument::Compact);
                clientSocket->write(sendData);
                qDebug() << "[服务器] 转发消息：" << QString::fromUtf8(sendData);
            }
        });

        // 客户端断开连接
        connect(clientSocket, &QTcpSocket::disconnected, this, [=]() {
            qDebug() << "[服务器] 客户端断开连接：" << clientSocket->peerAddress().toString();
            clientSocket->deleteLater();
        });
    }
};

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    TestServer server;
    return a.exec();
}

#include "test_server.moc"
