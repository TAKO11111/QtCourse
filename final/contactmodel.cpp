#include "contactmodel.h"
#include "dbmanager.h"
#include <QDebug>

ContactModel::ContactModel(QObject *parent) : QAbstractItemModel(parent) {}

QModelIndex ContactModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    // 父索引为空 → 是分组节点
    if (!parent.isValid()) {
        if (row < m_groups.size()) {
            return createIndex(row, column, (void*)&m_groups[row]);
        }
    } else {
        // 父索引不为空 → 是联系人节点（父节点是分组）
        ContactGroup* group = static_cast<ContactGroup*>(parent.internalPointer());
        if (row < group->contactList.size()) {
            return createIndex(row, column, (void*)&group->contactList[row]);
        }
    }

    return QModelIndex();
}

QModelIndex ContactModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) {
        return QModelIndex();
    }

    // 判断是否是联系人节点（需要返回父分组）
    QString* contact = static_cast<QString*>(index.internalPointer());
    if (contact) {
        // 遍历分组，找到该联系人所属的分组
        for (int i = 0; i < m_groups.size(); ++i) {
            if (m_groups[i].contactList.contains(*contact)) {
                return createIndex(i, index.column(), (void*)&m_groups[i]);
            }
        }
    }

    // 分组节点没有父节点
    return QModelIndex();
}

int ContactModel::rowCount(const QModelIndex &parent) const {
    // 父索引为空 → 返回分组数量
    if (!parent.isValid()) {
        return m_groups.size();
    }

    // 父索引是分组 → 返回该分组下的联系人数量
    ContactGroup* group = static_cast<ContactGroup*>(parent.internalPointer());
    if (group) {
        return group->contactList.size();
    }

    return 0;
}

int ContactModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1; // 只需要1列展示分组/联系人名称
}

QVariant ContactModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    // 分组节点
    ContactGroup* group = static_cast<ContactGroup*>(index.internalPointer());
    if (group && isGroupIndex(index)) {
        if (role == GroupNameRole || role == Qt::DisplayRole) {
            return group->groupName;
        }
    }

    // 联系人节点
    QString* contact = static_cast<QString*>(index.internalPointer());
    if (contact) {
        if (role == ContactNameRole || role == Qt::DisplayRole) {
            return *contact;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ContactModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[GroupNameRole] = "groupName";
    roles[ContactNameRole] = "contactName";
    return roles;
}

bool ContactModel::isGroupIndex(const QModelIndex& index) const {
    // 判断索引是否是分组（通过内部指针类型）
    void* ptr = index.internalPointer();
    if (!ptr) return false;

    // 遍历分组，判断指针是否指向某个分组
    for (const ContactGroup& group : m_groups) {
        if (&group == ptr) {
            return true;
        }
    }
    return false;
}

void ContactModel::loadContacts(const QString& username) {
    // 从数据库加载联系人
    QList<QVariantMap> dbResult = DBManager::getInstance().getContacts(username);
    if (dbResult.isEmpty()) {
        qDebug() << "[Model] 无联系人数据";
        return;
    }

    // 清空现有联系人
    clearContacts();

    // 解析数据库结果，按分组整理
    beginResetModel();
    QMap<QString, QList<QString>> groupMap;
    for (const QVariantMap& row : dbResult) {
        QString group = row["group_name"].toString();
        QString contact = row["contact_name"].toString();
        groupMap[group].append(contact);
    }

    // 转换为分组列表
    for (const QString& groupName : groupMap.keys()) {
        ContactGroup group;
        group.groupName = groupName;
        group.contactList = groupMap[groupName];
        m_groups.append(group);
    }
    endResetModel();

    qDebug() << "[Model] 加载联系人完成，共" << m_groups.size() << "个分组";
}

void ContactModel::addContact(const QString& username, const QString& contactName, const QString& group) {
    // 先保存到数据库
    bool saveOk = DBManager::getInstance().saveContact(username, contactName, group);
    if (!saveOk) {
        qDebug() << "[Model] 保存联系人失败";
        return;
    }

    // 更新模型
    beginResetModel();
    // 查找分组是否存在
    bool groupExist = false;
    for (ContactGroup& g : m_groups) {
        if (g.groupName == group) {
            g.contactList.append(contactName);
            groupExist = true;
            break;
        }
    }
    // 分组不存在则创建新分组
    if (!groupExist) {
        ContactGroup newGroup;
        newGroup.groupName = group;
        newGroup.contactList.append(contactName);
        m_groups.append(newGroup);
    }
    endResetModel();
}

void ContactModel::clearContacts() {
    beginResetModel();
    m_groups.clear();
    endResetModel();
}
