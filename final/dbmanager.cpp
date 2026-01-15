#include "dbmanager.h"
#include <QDebug>
#include <QDateTime>
#include <QSqlRecord>

// ========== 基础数据库操作实现 ==========
bool DBManager::initDB(const QString& dbPath) {
    QMutexLocker locker(&m_mutex); // 加锁：保证线程安全

    // 如果已初始化，直接返回成功
    if (m_isInit) return true;

    // 1. 创建SQLite连接（QSQLITE是Qt内置的SQLite驱动）
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath); // 数据库文件路径（当前目录下的im_system.db）

    // 2. 打开数据库
    if (!m_db.open()) {
        qDebug() << "[DB错误] 打开数据库失败：" << m_db.lastError().text();
        return false;
    }

    // 3. 创建核心表结构
    // 3.1 用户表（存储注册用户信息）
    QString createUserTable = R"(
        CREATE TABLE IF NOT EXISTS user (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,  -- 用户名（唯一）
            password TEXT NOT NULL,         -- 密码（实际项目需加密，此处简化）
            nickname TEXT NOT NULL,         -- 昵称
            create_time DATETIME DEFAULT CURRENT_TIMESTAMP  -- 创建时间
        );
    )";
    if (!execSql(createUserTable)) {
        qDebug() << "[DB错误] 创建用户表失败";
        return false;
    }

    // 3.2 聊天记录表（存储单聊/群聊记录）
    QString createChatTable = R"(
        CREATE TABLE IF NOT EXISTS chat_record (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            from_user TEXT NOT NULL,    -- 发送者用户名
            to_user TEXT NOT NULL,      -- 接收者/群ID
            content TEXT NOT NULL,      -- 消息内容
            is_group INTEGER DEFAULT 0, -- 是否群聊（0=否，1=是）
            send_time DATETIME DEFAULT CURRENT_TIMESTAMP -- 发送时间
        );
    )";
    if (!execSql(createChatTable)) {
        qDebug() << "[DB错误] 创建聊天记录表失败";
        return false;
    }

    // 3.3 联系人表（存储用户的联系人分组）
    QString createContactTable = R"(
        CREATE TABLE IF NOT EXISTS contact (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL,     -- 当前用户
            contact_name TEXT NOT NULL, -- 联系人用户名
            group_name TEXT DEFAULT '默认分组', -- 分组名称
            UNIQUE(username, contact_name) -- 保证一个用户的联系人不重复
        );
    )";
    if (!execSql(createContactTable)) {
        qDebug() << "[DB错误] 创建联系人表失败";
        return false;
    }

    m_isInit = true;
    qDebug() << "[DB成功] 数据库初始化完成，表结构创建成功";
    return true;
}

void DBManager::closeDB() {
    QMutexLocker locker(&m_mutex);
    if (m_db.isOpen()) {
        m_db.close();
        m_isInit = false;
        qDebug() << "[DB信息] 数据库连接已关闭";
    }
}

bool DBManager::execSql(const QString& sql, const QList<QVariant>& params) {
    QMutexLocker locker(&m_mutex);
    if (!m_db.isOpen()) {
        qDebug() << "[DB错误] 执行SQL失败：数据库未打开";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(sql); // 预处理SQL：防止SQL注入

    // 绑定参数（?占位符）
    for (int i = 0; i < params.size(); ++i) {
        query.bindValue(i, params[i]);
    }

    // 执行SQL
    if (!query.exec()) {
        qDebug() << "[DB错误] SQL执行失败：" << sql
                 << "错误信息：" << query.lastError().text();
        return false;
    }
    return true;
}

QList<QVariantMap> DBManager::querySql(const QString& sql, const QList<QVariant>& params) {
    QMutexLocker locker(&m_mutex);
    QList<QVariantMap> result; // 存储查询结果（键值对形式）

    if (!m_db.isOpen()) {
        qDebug() << "[DB错误] 查询SQL失败：数据库未打开";
        return result;
    }

    QSqlQuery query(m_db);
    query.prepare(sql);
    for (int i = 0; i < params.size(); ++i) {
        query.bindValue(i, params[i]);
    }

    if (!query.exec()) {
        qDebug() << "[DB错误] SQL查询失败：" << sql
                 << "错误信息：" << query.lastError().text();
        return result;
    }

    // 解析查询结果（将每行数据转为QVariantMap）
    QSqlRecord record = query.record();
    while (query.next()) {
        QVariantMap row;
        for (int i = 0; i < record.count(); ++i) {
            row.insert(record.fieldName(i), query.value(i));
        }
        result.append(row);
    }

    return result;
}

// ========== 业务操作实现 ==========
bool DBManager::registerUser(const QString& username, const QString& password, const QString& nickname) {
    // 检查用户名是否已存在
    QString checkSql = "SELECT username FROM user WHERE username = ?";
    QList<QVariantMap> checkResult = querySql(checkSql, {username});
    if (!checkResult.isEmpty()) {
        qDebug() << "[DB错误] 注册失败：用户名" << username << "已存在";
        return false;
    }

    // 插入新用户
    QString insertSql = "INSERT INTO user (username, password, nickname) VALUES (?, ?, ?)";
    return execSql(insertSql, {username, password, nickname});
}

bool DBManager::loginVerify(const QString& username, const QString& password) {
    QString sql = "SELECT id FROM user WHERE username = ? AND password = ?";
    QList<QVariantMap> result = querySql(sql, {username, password});
    return !result.isEmpty(); // 有结果则登录成功
}

bool DBManager::saveChatRecord(const QString& fromUser, const QString& toUser,
                               const QString& content, bool isGroup) {
    QString sql = "INSERT INTO chat_record (from_user, to_user, content, is_group) VALUES (?, ?, ?, ?)";
    return execSql(sql, {fromUser, toUser, content, isGroup ? 1 : 0});
}

QList<QVariantMap> DBManager::getChatRecords(const QString& fromUser, const QString& toUser,
                                             bool isGroup, int limit) {
    QString sql = R"(
        SELECT * FROM chat_record
        WHERE (from_user = ? AND to_user = ? AND is_group = ?)
        OR (from_user = ? AND to_user = ? AND is_group = ?)
        ORDER BY send_time DESC LIMIT ?
    )";
    // 双向查询：既能查自己发的，也能查对方发的
    return querySql(sql, {fromUser, toUser, isGroup ? 1 : 0,
                          toUser, fromUser, isGroup ? 1 : 0, limit});
}

bool DBManager::saveContact(const QString& username, const QString& contactName, const QString& group) {
    QString sql = "INSERT OR REPLACE INTO contact (username, contact_name, group_name) VALUES (?, ?, ?)";
    return execSql(sql, {username, contactName, group});
}

QList<QVariantMap> DBManager::getContacts(const QString& username) {
    QString sql = "SELECT * FROM contact WHERE username = ? ORDER BY group_name";
    return querySql(sql, {username});
}
