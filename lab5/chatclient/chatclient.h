#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>

class chatclient : public QObject
{
    Q_OBJECT
public:
    explicit chatclient(QObject *parent = nullptr);

signals:
    void connected();
    void messagereceived(const QString &text);
    void jsonreceived(const QJsonObject &docobj);
private:
    QTcpSocket *m_clientSocket;

public slots:
    void onreadyread();
    void sendmessage(const QString &text,const QString &type ="message");
    void connecttoserver(const QHostAddress &address,quint16 port);
    void disconnectfromhost();
};

#endif // CHATCLIENT_H
