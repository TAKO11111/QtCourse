#include "serverwork.h"
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

ServerWork::ServerWork(QObject *parent)
    : QObject{parent}
{
    m_serverSocket =new QTcpSocket(this);
    connect(m_serverSocket,&QTcpSocket::readyRead,this,&ServerWork::onreadyread);
    connect(m_serverSocket,&QTcpSocket::disconnected,this,&ServerWork::disconnectedfromclient);
}

bool ServerWork::setSocketDescriptor(qintptr socketDescriptor)
{
    return m_serverSocket->setSocketDescriptor(socketDescriptor);
}

void ServerWork::onreadyread()
{
    QByteArray jsondata;
    QDataStream socketStream(m_serverSocket);
    socketStream.setVersion(QDataStream::Qt_5_12);
    for(;;){
        socketStream.startTransaction();
        socketStream>>jsondata;
        if(socketStream.commitTransaction()){


            QJsonParseError parseerror;
            const QJsonDocument jsondoc=QJsonDocument::fromJson(jsondata,&parseerror);
            if(parseerror.error==QJsonParseError::NoError){
                if(jsondoc.isObject()){
                    emit logmessage(QJsonDocument(jsondoc).toJson(QJsonDocument::Compact));
                    emit jsonreceiver(this,jsondoc.object());
                }
            }
        }else{
            break;
        }
    }
}

void ServerWork::sendmessage(const QString &text, const QString &type)
{
    if(m_serverSocket->state()!=QAbstractSocket::ConnectedState)
        return;
    if(!text.isEmpty()){
        QDataStream serverStream(m_serverSocket);
        serverStream.setVersion(QDataStream::Qt_5_12);

        QJsonObject message;
        message["type"]=type;
        message["text"]=text;

        serverStream<<QJsonDocument(message).toJson();
    }
}

void ServerWork::sendjson(const QJsonObject &json)
{
    const QByteArray jsondata = QJsonDocument(json).toJson(QJsonDocument::Compact);
    emit    logmessage(QLatin1String("Sending to")+username()+QLatin1String("-")+QString::fromUtf8(jsondata));
    QDataStream sockerstream(m_serverSocket);
    sockerstream.setVersion(QDataStream::Qt_5_7);
    sockerstream<<jsondata;
}
QString ServerWork::username()
{
    return m_username;
}

void ServerWork::setusername(QString user)
{
    m_username=user;
}
