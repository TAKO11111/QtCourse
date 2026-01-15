#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QAbstractItemModel>
#include <QVariant>
#include <QList>
#include <QHash>

struct ContactGroup {
    ContactGroup() = default;
    ContactGroup(const QString& name, const QList<QString>& contacts = {})
        : groupName(name), contactList(contacts) {}

    QString groupName;
    QList<QString> contactList;
};

class ContactModel : public QAbstractItemModel {
    Q_OBJECT
public:
    enum ContactRoles {
        GroupNameRole = Qt::UserRole + 1,
        ContactNameRole
    };

    explicit ContactModel(QObject *parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void loadContacts(const QString& username);
    void addContact(const QString& username, const QString& contactName, const QString& group = "默认分组");
    void clearContacts();
    QList<ContactGroup> getGroups() const { return m_groups; }
    bool isGroupIndex(const QModelIndex& index) const;

private:
    QList<ContactGroup> m_groups;
    bool isGroup(const QModelIndex& index) const;
    QPair<int, int> getIndexPos(const QModelIndex& index) const;
};

#endif // CONTACTMODEL_H
