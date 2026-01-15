#ifndef TCPMESSAGE_H
#define TCPMESSAGE_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

// 消息类型枚举（区分不同业务的消息）
enum class MessageType {
    Login = 1,        // 登录消息
    Register,         // 注册消息
    SingleChat,       // 单聊消息
    GroupChat,        // 群聊消息
    Heartbeat,        // 心跳包
    ContactList,      // 获取联系人列表
    Error,            // 错误消息
    Unknown           // 未知消息
};

// 消息封装类：负责消息的编码（对象→JSON）和解码（JSON→对象）
class TcpMessage {
public:
    // 构造函数
    TcpMessage() = default;
    TcpMessage(MessageType type, const QString& from, const QString& to, const QString& content);

    // 编码：将消息转为JSON字符串（用于发送）
    QString toJsonString() const;
    // 解码：从JSON字符串解析为消息对象
    static TcpMessage fromJsonString(const QString& jsonStr);

    // Getter/Setter
    MessageType type() const { return m_type; }
    void setType(MessageType type) { m_type = type; }

    QString from() const { return m_from; }
    void setFrom(const QString& from) { m_from = from; }

    QString to() const { return m_to; }
    void setTo(const QString& to) { m_to = to; }

    QString content() const { return m_content; }
    void setContent(const QString& content) { m_content = content; }

    QString time() const { return m_time; }
    void setTime(const QString& time) { m_time = time; }

private:
    MessageType m_type = MessageType::Unknown; // 消息类型
    QString m_from;                            // 发送者
    QString m_to;                              // 接收者/群ID
    QString m_content;                         // 消息内容
    QString m_time;                            // 消息时间（默认当前时间）
};

#endif // TCPMESSAGE_H
