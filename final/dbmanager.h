#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QVariantMap>
#include <QList>

class DBManager {
public:
    static DBManager& getInstance();
    bool initDB();
    QString lastError() const { return m_lastError; }

    bool registerUser(const QString& username, const QString& password, const QString& nickname);
    bool loginVerify(const QString& username, const QString& password);

    bool saveChatRecord(const QString& fromUser, const QString& toUser, const QString& content, bool isGroup = false);
    QList<QVariantMap> getChatRecords(const QString& fromUser, const QString& toUser, bool isGroup = false);

    QList<QVariantMap> getContacts(const QString& username);

private:
    DBManager() = default;
    ~DBManager() = default;
    DBManager(const DBManager&) = delete;
    DBManager& operator=(const DBManager&) = delete;

    bool execSql(const QString& sql, const QList<QVariant>& params = {});
    QList<QVariantMap> querySql(const QString& sql, const QList<QVariant>& params = {});

    QSqlDatabase m_db;
    QString m_lastError;
};

#endif // DBMANAGER_H
