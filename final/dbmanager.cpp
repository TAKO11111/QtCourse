#include "dbmanager.h"
#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlRecord>

DBManager& DBManager::getInstance() {
    static DBManager instance;
    return instance;
}

bool DBManager::initDB() {
    QString dbPath = QDir::currentPath() + "/im_system.db";
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        qDebug() << "[DB] Open failed:" << m_lastError;
        return false;
    }

    QString createUser = R"(
        CREATE TABLE IF NOT EXISTS user (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            nickname TEXT NOT NULL
        )
    )";
    if (!execSql(createUser)) return false;

    QString createChat = R"(
        CREATE TABLE IF NOT EXISTS chat_record (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            from_user TEXT NOT NULL,
            to_user TEXT NOT NULL,
            content TEXT NOT NULL,
            is_group INTEGER DEFAULT 0,
            send_time DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";
    if (!execSql(createChat)) return false;

    QString createContact = R"(
        CREATE TABLE IF NOT EXISTS contact (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL,
            contact_name TEXT NOT NULL,
            group_name TEXT DEFAULT '默认分组',
            UNIQUE(username, contact_name)
        )
    )";
    if (!execSql(createContact)) return false;

    return true;
}

bool DBManager::registerUser(const QString& username, const QString& password, const QString& nickname) {
    QString sql = "INSERT INTO user (username, password, nickname) VALUES (?, ?, ?)";
    return execSql(sql, {username, password, nickname});
}

bool DBManager::loginVerify(const QString& username, const QString& password) {
    QString sql = "SELECT id FROM user WHERE username = ? AND password = ?";
    QList<QVariantMap> res = querySql(sql, {username, password});
    return !res.isEmpty();
}

bool DBManager::saveChatRecord(const QString& fromUser, const QString& toUser, const QString& content, bool isGroup) {
    QString sql = "INSERT INTO chat_record (from_user, to_user, content, is_group) VALUES (?, ?, ?, ?)";
    return execSql(sql, {fromUser, toUser, content, isGroup ? 1 : 0});
}

QList<QVariantMap> DBManager::getChatRecords(const QString& fromUser, const QString& toUser, bool isGroup) {
    QString sql = R"(
        SELECT from_user, to_user, content, send_time
        FROM chat_record
        WHERE (from_user = ? AND to_user = ? AND is_group = ?)
        OR (from_user = ? AND to_user = ? AND is_group = ?)
        ORDER BY send_time DESC
        LIMIT 100
    )";
    return querySql(sql, {fromUser, toUser, isGroup ? 1 : 0, toUser, fromUser, isGroup ? 1 : 0});
}

QList<QVariantMap> DBManager::getContacts(const QString& username) {
    QString sql = "SELECT contact_name, group_name FROM contact WHERE username = ?";
    return querySql(sql, {username});
}

bool DBManager::execSql(const QString& sql, const QList<QVariant>& params) {
    QSqlQuery query;
    query.prepare(sql);
    for (const auto& p : params) {
        query.addBindValue(p);
    }
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "[DB] Exec failed:" << m_lastError << "SQL:" << sql;
        return false;
    }
    return true;
}

QList<QVariantMap> DBManager::querySql(const QString& sql, const QList<QVariant>& params) {
    QList<QVariantMap> result;
    QSqlQuery query;
    query.prepare(sql);
    for (const auto& p : params) {
        query.addBindValue(p);
    }
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "[DB] Query failed:" << m_lastError << "SQL:" << sql;
        return result;
    }

    QSqlRecord rec = query.record();
    while (query.next()) {
        QVariantMap map;
        for (int i = 0; i < rec.count(); i++) {
            map[rec.fieldName(i)] = query.value(i);
        }
        result.append(map);
    }
    return result;
}
