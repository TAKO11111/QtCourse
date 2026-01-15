#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QAbstractItemModel>
#include <QVariant>
#include <QList>
#include <QMap>

// 联系人分组结构
struct ContactGroup {
    QString groupName;                // 分组名称
    QList<QString> contactList;       // 该分组下的联系人列表
};

// 联系人模型（适配QTreeView，展示分组+联系人）
class ContactModel : public QAbstractItemModel {
    Q_OBJECT
public:
    // 自定义角色
    enum ContactRoles {
        GroupNameRole = Qt::UserRole + 1,
        ContactNameRole
    };

    explicit ContactModel(QObject *parent = nullptr);

    // 重写QAbstractItemModel纯虚函数
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // 业务方法
    void loadContacts(const QString& username); // 加载指定用户的联系人
    void addContact(const QString& username, const QString& contactName, const QString& group = "默认分组"); // 添加联系人
    void clearContacts(); // 清空联系人

private:
    QList<ContactGroup> m_groups; // 联系人分组列表
    // 辅助方法：根据索引判断是分组还是联系人
    bool isGroupIndex(const QModelIndex& index) const;
};

#endif // CONTACTMODEL_H
