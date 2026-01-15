#ifndef DBMANAGER_H
#define DBMANAGER_H

// 引入Qt核心头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QList>
#include <QVariantMap>
#include <QMutex>
#include <QMutexLocker>

// 单例模式的数据库管理类：封装所有SQLite操作
class DBManager {
public:
    // 获取单例实例（线程安全）
    static DBManager& getInstance() {
        // 静态局部变量，保证仅初始化一次
        static DBManager instance;
        return instance;
    }

    // ========== 基础数据库操作 ==========
    // 初始化数据库（创建表结构）
    bool initDB(const QString& dbPath = "./im_system.db");
    // 关闭数据库连接
    void closeDB();
    // 通用执行SQL（增/删/改）
    bool execSql(const QString& sql, const QList<QVariant>& params = {});
    // 通用查询SQL（查）
    QList<QVariantMap> querySql(const QString& sql, const QList<QVariant>& params = {});

    // ========== 业务相关数据库操作 ==========
    // 1. 用户注册
    bool registerUser(const QString& username, const QString& password, const QString& nickname);
    // 2. 用户登录验证（返回是否存在该用户）
    bool loginVerify(const QString& username, const QString& password);
    // 3. 保存聊天记录（isGroup：是否群聊）
    bool saveChatRecord(const QString& fromUser, const QString& toUser,
                        const QString& content, bool isGroup = false);
    // 4. 获取聊天记录（按时间倒序）
    QList<QVariantMap> getChatRecords(const QString& fromUser, const QString& toUser,
                                      bool isGroup = false, int limit = 100);
    // 5. 保存联系人（可指定分组）
    bool saveContact(const QString& username, const QString& contactName,
                     const QString& group = "默认分组");
    // 6. 获取联系人列表（按分组展示）
    QList<QVariantMap> getContacts(const QString& username);

private:
    // 私有构造/析构：禁止外部实例化
    DBManager() = default;
    ~DBManager() { closeDB(); }

    // 禁止拷贝和赋值：保证单例唯一性
    DBManager(const DBManager&) = delete;
    DBManager& operator=(const DBManager&) = delete;

    // 成员变量
    QSqlDatabase m_db;       // 数据库连接对象
    QMutex m_mutex;          // 互斥锁：保证多线程操作数据库安全
    bool m_isInit = false;   // 数据库是否已初始化
};

#endif // DBMANAGER_H
