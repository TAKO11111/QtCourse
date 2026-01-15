#ifndef TCPMESSAGE_H
#define TCPMESSAGE_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QJsonObject>

enum class MessageType {
    SingleChat = 1,
    GroupChat = 2,
    Heartbeat = 3,
    Login = 4,
    Register = 5
};

enum class MessageRoleType {
    Send = 1,
    Receive = 2
};

struct MessageItem {
    QString content;
    QDateTime time;
    MessageRoleType type;
    QString fromUser;
    QString toUser;
};

class TcpMessage
{
public:
    TcpMessage();
    TcpMessage(MessageType type, const QString& from, const QString& to, const QString& content);

    QJsonObject toJson() const;
    static TcpMessage fromJson(const QJsonObject& obj);
    static TcpMessage fromJsonString(const QString& jsonStr);

    MessageType type() const { return m_type; }
    QString from() const { return m_from; }
    QString to() const { return m_to; }
    QString content() const { return m_content; }
    QDateTime time() const { return m_time; }

private:
    MessageType m_type;
    QString m_from;
    QString m_to;
    QString m_content;
    QDateTime m_time;
};

#endif // TCPMESSAGE_H
