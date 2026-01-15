#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QString>

// 消息类型枚举（区分发送/接收/系统消息）
enum class MessageRoleType {
    Send = 0,    // 自己发送的消息
    Receive = 1, // 接收的消息
    System = 2   // 系统消息（如连接提示）
};

// 单条消息的数据结构
struct MessageItem {
    QString content;      // 消息内容
    QString time;         // 发送时间
    MessageRoleType type; // 消息类型
    QString fromUser;     // 发送者用户名
    QString toUser;       // 接收者用户名/群ID
};

// 消息列表模型（适配QListView）
class MessageModel : public QAbstractListModel {
    Q_OBJECT
public:
    // 自定义角色（用于View获取数据）
    enum MessageRoles {
        ContentRole = Qt::UserRole + 1,
        TimeRole,
        TypeRole,
        FromUserRole,
        ToUserRole
    };

    explicit MessageModel(QObject *parent = nullptr);

    // 重写QAbstractListModel的纯虚函数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // 业务方法
    void addMessage(const MessageItem& item); // 添加单条消息
    void clearMessages();                     // 清空所有消息
    void loadHistoryMessages(const QString& fromUser, const QString& toUser, bool isGroup = false); // 加载历史消息
    QList<MessageItem> getAllMessages() const; // 获取所有消息

private:
    QList<MessageItem> m_messageList; // 消息列表数据
};

#endif // MESSAGEMODEL_H
