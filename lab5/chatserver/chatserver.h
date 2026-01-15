#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QQmlEngine>
#include <QTcpServer>
#include <serverwork.h>
#include <QJsonObject>

class chatserver : public QTcpServer
{
    QML_ELEMENT
    Q_OBJECT

public:
    explicit chatserver(QObject *parent=nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
    void broadcast(const QJsonObject &message,ServerWork *exclude);
signals:
    void logMessage(const QString &msg);
public slots:
    void stopServer();
    void jsonreceiver(ServerWork *sender,const QJsonObject &docobj);
    void userdisconnected(ServerWork *sender);
private:
    QVector<ServerWork*>m_clients;


};

#endif // CHATSERVER_H
