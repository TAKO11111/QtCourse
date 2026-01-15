#include "tcpmessage.h"
#include <QDateTime>

TcpMessage::TcpMessage(MessageType type, const QString& from, const QString& to, const QString& content) {
    m_type = type;
    m_from = from;
    m_to = to;
    m_content = content;
    // 默认设置为当前时间（格式：yyyy-MM-dd hh:mm:ss）
    m_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

QString TcpMessage::toJsonString() const {
    QJsonObject jsonObj;
    // 消息类型转为整数存储
    jsonObj["type"] = static_cast<int>(m_type);
    jsonObj["from"] = m_from;
    jsonObj["to"] = m_to;
    jsonObj["content"] = m_content;
    jsonObj["time"] = m_time;

    // 转为JSON字符串
    QJsonDocument doc(jsonObj);
    return doc.toJson(QJsonDocument::Compact); // 紧凑格式，减少传输体积
}

TcpMessage TcpMessage::fromJsonString(const QString& jsonStr) {
    TcpMessage msg;
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    if (!doc.isObject()) {
        return msg; // 解析失败，返回空消息
    }

    QJsonObject jsonObj = doc.object();
    // 解析各字段
    msg.setType(static_cast<MessageType>(jsonObj["type"].toInt()));
    msg.setFrom(jsonObj["from"].toString());
    msg.setTo(jsonObj["to"].toString());
    msg.setContent(jsonObj["content"].toString());
    msg.setTime(jsonObj["time"].toString());

    return msg;
}
