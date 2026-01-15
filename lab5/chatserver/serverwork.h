#ifndef SERVERWORK_H
#define SERVERWORK_H

#include <QObject>
#include <qtcpsocket.h>
#include <QJsonObject>

class ServerWork : public QObject
{
    Q_OBJECT
public:
    explicit ServerWork(QObject *parent = nullptr);
    virtual bool setSocketDescriptor(qintptr socketDescriptor);
    QString username();
    void setusername(QString user);

signals:
    void logmessage(const QString &mas);
    void jsonreceiver(ServerWork *sender,const QJsonObject &docobj);
    void disconnectedfromclient();

private:
    QTcpSocket *m_serverSocket;
    QString m_username;

public slots:
    void onreadyread();
    void sendmessage(const QString &text,const QString &type ="message");
    void sendjson(const QJsonObject &json);
};

#endif // SERVERWORK_H
