#include "messagemodel.h"
#include "dbmanager.h"
#include <QDebug>
#include <algorithm>

MessageModel::MessageModel(QObject *parent) : QAbstractListModel(parent) {}

int MessageModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_messageList.size();
}

QVariant MessageModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_messageList.size()) {
        return QVariant();
    }

    const MessageItem& item = m_messageList[index.row()];
    switch (role) {
    case ContentRole:
        return item.content;
    case TimeRole:
        return item.time.toString("HH:mm:ss");
    case TypeRole: // 匹配头文件的TypeRole，对应item.type
        return static_cast<int>(item.type);
    case FromUserRole:
        return item.fromUser;
    case ToUserRole:
        return item.toUser;
    case Qt::DisplayRole:
        return QString("%1: %2").arg(item.fromUser).arg(item.content);
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> MessageModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ContentRole] = "content";
    roles[TimeRole] = "time";
    roles[TypeRole] = "type"; // 匹配TypeRole，和mainwindow中的绑定一致
    roles[FromUserRole] = "fromUser";
    roles[ToUserRole] = "toUser";
    return roles;
}

void MessageModel::addMessage(const MessageItem& item) {
    beginInsertRows(QModelIndex(), m_messageList.size(), m_messageList.size());
    m_messageList.append(item);
    endInsertRows();

    // 滚动到最后一条消息
    QModelIndex lastIndex = index(m_messageList.size() - 1, 0);
    emit dataChanged(lastIndex, lastIndex);

    qDebug() << "MessageModel: 添加消息 - 来自:" << item.fromUser
             << ", 内容:" << item.content
             << ", 类型:" << static_cast<int>(item.type);
}

void MessageModel::clearMessages() {
    beginResetModel();
    m_messageList.clear();
    endResetModel();
    qDebug() << "MessageModel: 清除所有消息";
}

void MessageModel::loadHistoryMessages(const QString& fromUser, const QString& toUser, bool isGroup) {
    Q_UNUSED(isGroup);
    qDebug() << "MessageModel: 加载历史消息 - 从" << fromUser << "到" << toUser;

    clearMessages();
    beginResetModel();

    QList<QVariantMap> records = DBManager::getInstance().getChatRecords(fromUser, toUser);
    std::reverse(records.begin(), records.end());

    qDebug() << "从数据库加载到" << records.size() << "条记录";

    for (const QVariantMap& rec : records) {
        MessageItem item;
        item.content = rec["content"].toString();
        item.time = QDateTime::fromString(rec["send_time"].toString(), "yyyy-MM-dd HH:mm:ss");
        if (!item.time.isValid()) {
            item.time = QDateTime::currentDateTime();
        }
        item.fromUser = rec["from_user"].toString();
        item.toUser = rec["to_user"].toString();
        item.type = (item.fromUser == fromUser) ? MessageRoleType::Send : MessageRoleType::Receive;

        m_messageList.append(item);

        qDebug() << "加载消息 - 来自:" << item.fromUser
                 << ", 到:" << item.toUser
                 << ", 内容:" << item.content;
    }

    endResetModel();

    // 通知视图更新
    if (!m_messageList.isEmpty()) {
        QModelIndex first = index(0, 0);
        QModelIndex last = index(m_messageList.size() - 1, 0);
        emit dataChanged(first, last);
    }
}
