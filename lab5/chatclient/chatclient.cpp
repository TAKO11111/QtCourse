#include "chatclient.h"
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

chatclient::chatclient(QObject *parent)
    : QObject{parent}
{
    m_clientSocket =new QTcpSocket(this);
    connect(m_clientSocket,&QTcpSocket::connected,this,&chatclient::connected);
    connect(m_clientSocket,&QTcpSocket::readyRead,this,&chatclient::onreadyread);
}

void chatclient::onreadyread()
{
    QByteArray jsondata;
    QDataStream socketStream(m_clientSocket);
    socketStream.setVersion(QDataStream::Qt_5_12);
    for(;;){
        socketStream.startTransaction();
        socketStream>>jsondata;
        if(socketStream.commitTransaction()){
            // emit messagereceived(QString::fromUtf8(jsondata));
            QJsonParseError parseerror;
            const QJsonDocument jsondoc=QJsonDocument::fromJson(jsondata,&parseerror);
            if(parseerror.error==QJsonParseError::NoError){
                if(jsondoc.isObject()){
                    // emit logmessage(QJsonDocument(jsondoc).toJson(QJsonDocument::Compact));
                    emit jsonreceived(jsondoc.object());
                }
            }
        }else{
            break;
        }
    }
}

void chatclient::sendmessage(const QString &text, const QString &type)
{
    if(m_clientSocket->state()!=QAbstractSocket::ConnectedState)
        return;
    if(!text.isEmpty()){
        QDataStream serverStream(m_clientSocket);
        serverStream.setVersion(QDataStream::Qt_5_12);

        QJsonObject message;
        message["type"]=type;
        message["text"]=text;

        serverStream<<QJsonDocument(message).toJson();
    }
}

void chatclient::connecttoserver(const QHostAddress &address, quint16 port)
{
    m_clientSocket->connectToHost(address,port);
}

void chatclient::disconnectfromhost()
{
    m_clientSocket->disconnectFromHost();
}
