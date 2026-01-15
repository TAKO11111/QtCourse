#include "tcpmessage.h"
#include <QJsonDocument>

TcpMessage::TcpMessage()
    : m_type(MessageType::SingleChat), m_time(QDateTime::currentDateTime())
{}

TcpMessage::TcpMessage(MessageType type, const QString& from, const QString& to, const QString& content)
    : m_type(type), m_from(from), m_to(to), m_content(content), m_time(QDateTime::currentDateTime())
{}

QJsonObject TcpMessage::toJson() const {
    QJsonObject obj;
    obj["type"] = static_cast<int>(m_type);
    obj["from"] = m_from;
    obj["to"] = m_to;
    obj["content"] = m_content;
    obj["time"] = m_time.toString("yyyy-MM-dd HH:mm:ss");
    return obj;
}

TcpMessage TcpMessage::fromJson(const QJsonObject& obj) {
    TcpMessage msg;
    msg.m_type = static_cast<MessageType>(obj["type"].toInt());
    msg.m_from = obj["from"].toString();
    msg.m_to = obj["to"].toString();
    msg.m_content = obj["content"].toString();
    msg.m_time = QDateTime::fromString(obj["time"].toString(), "yyyy-MM-dd HH:mm:ss");
    return msg;
}

TcpMessage TcpMessage::fromJsonString(const QString& jsonStr) {
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    return fromJson(doc.object());
}
