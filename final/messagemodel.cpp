#include "messagemodel.h"
#include "dbmanager.h"
#include <QDebug>

MessageModel::MessageModel(QObject *parent) : QAbstractListModel(parent) {}

int MessageModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_messageList.size(); // 行数 = 消息数量
}

QVariant MessageModel::data(const QModelIndex &index, int role) const {
    // 检查索引有效性
    if (!index.isValid() || index.row() >= m_messageList.size()) {
        return QVariant();
    }

    const MessageItem& item = m_messageList[index.row()];
    switch (role) {
    case ContentRole:
        return item.content;
    case TimeRole:
        return item.time;
    case TypeRole:
        return static_cast<int>(item.type);
    case FromUserRole:
        return item.fromUser;
    case ToUserRole:
        return item.toUser;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> MessageModel::roleNames() const {
    // 映射角色名（QML中会用到，C++中也可通过角色名获取）
    QHash<int, QByteArray> roles;
    roles[ContentRole] = "content";
    roles[TimeRole] = "time";
    roles[TypeRole] = "type";
    roles[FromUserRole] = "fromUser";
    roles[ToUserRole] = "toUser";
    return roles;
}

void MessageModel::addMessage(const MessageItem& item) {
    // 添加消息前通知View：开始插入行
    beginInsertRows(QModelIndex(), m_messageList.size(), m_messageList.size());
    m_messageList.append(item);
    // 通知View：插入完成
    endInsertRows();
}

void MessageModel::clearMessages() {
    // 清空消息前通知View：开始重置模型
    beginResetModel();
    m_messageList.clear();
    // 通知View：重置完成
    endResetModel();
}

void MessageModel::loadHistoryMessages(const QString& fromUser, const QString& toUser, bool isGroup) {
    // 从数据库加载历史消息
    QList<QVariantMap> dbResult = DBManager::getInstance().getChatRecords(fromUser, toUser, isGroup);
    if (dbResult.isEmpty()) {
        qDebug() << "[Model] 无历史聊天记录";
        return;
    }

    // 清空现有消息
    clearMessages();

    // 解析数据库结果并添加到模型
    beginResetModel();
    for (const QVariantMap& row : dbResult) {
        MessageItem item;
        item.content = row["content"].toString();
        item.time = row["send_time"].toString();
        item.fromUser = row["from_user"].toString();
        item.toUser = row["to_user"].toString();

        // 判断消息类型（自己发送/接收）
        if (item.fromUser == fromUser) {
            item.type = MessageRoleType::Send;
        } else {
            item.type = MessageRoleType::Receive;
        }

        m_messageList.append(item);
    }
    endResetModel();

    qDebug() << "[Model] 加载历史消息完成，共" << m_messageList.size() << "条";
}

QList<MessageItem> MessageModel::getAllMessages() const {
    return m_messageList;
}
