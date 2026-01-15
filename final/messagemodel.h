#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include <QDateTime>
#include <QList>
// 引入tcpmessage.h，复用其中的枚举/结构体，避免重复定义
#include "tcpmessage.h"

// 注意：删除此处重复的 MessageRoleType/MessageItem 定义，直接用 tcpmessage.h 中的

class MessageModel : public QAbstractListModel
{
    Q_OBJECT
public:
    // 数据角色定义（和你的cpp调用匹配）
    enum MessageRole {
        ContentRole = Qt::UserRole + 1,
        TimeRole,
        TypeRole,  // 对应mainwindow中用的item.type
        FromUserRole,
        ToUserRole
    };

    explicit MessageModel(QObject *parent = nullptr);

    // 重写QAbstractListModel的核心接口
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // 必须声明mainwindow中调用的所有方法
    void addMessage(const MessageItem &item);
    void clearMessages();
    void loadHistoryMessages(const QString& fromUser, const QString& toUser, bool isGroup = false);

private:
    QList<MessageItem> m_messageList; // 存储消息的列表
};

#endif // MESSAGEMODEL_H
