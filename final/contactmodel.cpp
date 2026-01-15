#include "contactmodel.h"
#include <QDebug>

ContactModel::ContactModel(QObject *parent) : QAbstractItemModel(parent) {}

QModelIndex ContactModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent)) return QModelIndex();

    if (!parent.isValid()) {
        if (row < m_groups.size()) {
            quint64 id = static_cast<quint64>(row) << 16;
            return createIndex(row, column, id);
        }
    } else {
        int groupIdx = parent.internalId() >> 16;
        if (groupIdx >= 0 && groupIdx < m_groups.size()) {
            const ContactGroup& group = m_groups[groupIdx];
            if (row < group.contactList.size()) {
                quint64 id = (static_cast<quint64>(groupIdx) << 16) | (row + 1);
                return createIndex(row, column, id);
            }
        }
    }
    return QModelIndex();
}

QModelIndex ContactModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) return QModelIndex();

    quint64 id = index.internalId();
    int groupIdx = id >> 16;
    int contactIdx = id & 0xFFFF;

    if (contactIdx != 0 && groupIdx < m_groups.size()) {
        quint64 parentId = static_cast<quint64>(groupIdx) << 16;
        return createIndex(groupIdx, index.column(), parentId);
    }
    return QModelIndex();
}

int ContactModel::rowCount(const QModelIndex &parent) const {
    if (!parent.isValid()) {
        return m_groups.size();
    } else {
        int groupIdx = parent.internalId() >> 16;
        if (groupIdx >= 0 && groupIdx < m_groups.size()) {
            return m_groups[groupIdx].contactList.size();
        }
    }
    return 0;
}

int ContactModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}

QVariant ContactModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();

    quint64 id = index.internalId();
    int groupIdx = id >> 16;
    int contactIdx = id & 0xFFFF;

    if (contactIdx == 0) {
        if (groupIdx < m_groups.size()) {
            if (role == GroupNameRole || role == Qt::DisplayRole) {
                return m_groups[groupIdx].groupName;
            }
        }
    } else {
        if (groupIdx < m_groups.size()) {
            const ContactGroup& group = m_groups[groupIdx];
            if ((contactIdx - 1) < group.contactList.size()) {
                if (role == ContactNameRole || role == Qt::DisplayRole) {
                    return group.contactList[contactIdx - 1];
                }
            }
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

void ContactModel::loadContacts(const QString& username) {
    clearContacts();
    beginResetModel();
    m_groups.append(ContactGroup("默认分组", {"user1", "user2", "user3"}));
    m_groups.append(ContactGroup("好友", {"user4", "user5"}));
    endResetModel();
}

void ContactModel::addContact(const QString& username, const QString& contactName, const QString& group) {
    Q_UNUSED(username);
    beginResetModel();
    bool found = false;
    for (auto& g : m_groups) {
        if (g.groupName == group) {
            g.contactList.append(contactName);
            found = true;
            break;
        }
    }
    if (!found) {
        m_groups.append(ContactGroup(group, {contactName}));
    }
    endResetModel();
}

void ContactModel::clearContacts() {
    beginResetModel();
    m_groups.clear();
    endResetModel();
}

bool ContactModel::isGroupIndex(const QModelIndex& index) const {
    if (!index.isValid()) return false;
    return (index.internalId() & 0xFFFF) == 0;
}

bool ContactModel::isGroup(const QModelIndex& index) const {
    return isGroupIndex(index);
}

QPair<int, int> ContactModel::getIndexPos(const QModelIndex& index) const {
    if (!index.isValid()) return {-1, -1};
    quint64 id = index.internalId();
    return {id >> 16, id & 0xFFFF};
}
