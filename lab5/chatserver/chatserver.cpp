#include "chatserver.h"
#include "serverwork.h"
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

chatserver::chatserver(QObject *parent)
    :QTcpServer(parent)
{

}


void chatserver::incomingConnection(qintptr socketDescriptor)
{
    ServerWork *work =new ServerWork(this);
    if(!work->setSocketDescriptor(socketDescriptor)){
        work->deleteLater();
        return;
    }
    connect(work,&ServerWork::logmessage,this,&chatserver::logMessage);
    connect(work,&ServerWork::jsonreceiver,this,&chatserver::jsonreceiver);
    connect(work,&ServerWork::disconnectedfromclient,this,std::bind(&chatserver::userdisconnected,this,work));
    m_clients.append(work);
    emit logMessage("新的用户连接上了");
}

void chatserver::broadcast(const QJsonObject &message, ServerWork *exclude)
{
    for(ServerWork *work:m_clients){
        work->sendjson(message);
    }
}

void chatserver::stopServer()
{
    close();
}

void chatserver::jsonreceiver(ServerWork *sender, const QJsonObject &docobj)
{
    const QJsonValue typeval=docobj.value("type");
    if(typeval.isNull()||!typeval.isString())
        return;
    if(typeval.toString().compare("message",Qt::CaseInsensitive)==0){
        const QJsonValue textval=docobj.value("text");
        if(textval.isNull()||!textval.isString())
            return;
        const QString text=textval.toString().trimmed();
        if(text.isEmpty())
            return;
        QJsonObject message;
        message[QStringLiteral("type")]="message";
        message[QStringLiteral("text")]=text;
        message[QStringLiteral("sender")]=sender->username();
        broadcast(message,sender);

    }
    else if(typeval.toString().compare("login",Qt::CaseInsensitive)==0){
        const QJsonValue usernameval=docobj.value("text");
        if(usernameval.isNull()||!usernameval.isString())
            return;
        sender->setusername(usernameval.toString());
        QJsonObject connectedmessage;
        connectedmessage["type"]="newuser";
        connectedmessage["username"]=usernameval.toString();
        broadcast(connectedmessage,sender);

        QJsonObject userlistmessage;
        userlistmessage["type"]="userlist";
        QJsonArray userlist;
        for(ServerWork *work:m_clients){
            if(work==sender)
                userlist.append(work->username()+"*");
            else
                userlist.append(work->username());
        }
        userlistmessage["userlist"]=userlist;
        sender->sendjson(userlistmessage);

    }
}

void chatserver::userdisconnected(ServerWork *sender)
{
    m_clients.removeAll(sender);
    const QString username =sender->username();
    if(!username.isEmpty()){
        QJsonObject disconnectedmaessage;
        disconnectedmaessage["type"]="userdisconnected";
        disconnectedmaessage["username"]=username;
        broadcast(disconnectedmaessage,nullptr);
        emit logMessage(username+"disconnected");
    }
    sender->deleteLater();
}
