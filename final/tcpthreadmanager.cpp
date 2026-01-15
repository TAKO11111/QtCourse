#include "tcpthreadmanager.h"
#include <QDebug>
#include <QCoreApplication>

TcpThreadManager& TcpThreadManager::getInstance() {
    static TcpThreadManager instance;
    return instance;
}

TcpThreadManager::TcpThreadManager(QObject *parent) : QObject(parent),
    m_thread(nullptr),
    m_tcpClient(nullptr),
    m_initialized(false)
{
    qDebug() << "TcpThreadManager 构造函数 - 当前线程:" << QThread::currentThread();
    initClient();
}

TcpThreadManager::~TcpThreadManager() {
    qDebug() << "TcpThreadManager 开始析构";

    QMutexLocker locker(&m_mutex);

    if (m_tcpClient) {
        // 直接调用，因为对象还在同一个线程
        m_tcpClient->disconnectFromServer();

        // 删除对象（会在正确线程中删除）
        m_tcpClient->deleteLater();
        m_tcpClient = nullptr;
    }

    if (m_thread) {
        m_thread->quit();
        if (!m_thread->wait(2000)) {
            m_thread->terminate();
            m_thread->wait();
        }
        delete m_thread;
        m_thread = nullptr;
    }

    qDebug() << "TcpThreadManager 析构完成";
}

void TcpThreadManager::initClient() {
    QMutexLocker locker(&m_mutex);

    if (m_initialized) return;

    qDebug() << "初始化TcpClient...";

    // 创建工作线程
    m_thread = new QThread();
    m_thread->setObjectName("TcpClientThread");

    // 创建TcpClient（无父对象）
    m_tcpClient = new TcpClient();

    qDebug() << "移动TcpClient到工作线程...";
    m_tcpClient->moveToThread(m_thread);

    // 连接信号槽
    connect(m_tcpClient, &TcpClient::messageReceived,
            this, &TcpThreadManager::messageReceived,
            Qt::QueuedConnection);

    connect(m_tcpClient, &TcpClient::connectStateChanged,
            this, &TcpThreadManager::connectStateChanged,
            Qt::QueuedConnection);

    connect(m_tcpClient, &TcpClient::errorOccurred,
            this, &TcpThreadManager::errorOccurred,
            Qt::QueuedConnection);

    // 启动线程
    m_thread->start();

    // 等待线程启动
    QThread::msleep(100);

    m_initialized = true;
    qDebug() << "TcpThreadManager 初始化完成";
}

bool TcpThreadManager::connectToServer(const QString& ip, quint16 port) {
    qDebug() << "TcpThreadManager::connectToServer - 尝试连接服务器" << ip << ":" << port;

    if (!m_tcpClient || !m_thread || !m_thread->isRunning()) {
        qDebug() << "错误：TcpClient未初始化或线程未运行";
        return false;
    }

    bool result = false;
    QMetaObject::invokeMethod(m_tcpClient, [this, ip, port, &result]() {
        result = m_tcpClient->connectToServer(ip, port);
    }, Qt::BlockingQueuedConnection);

    qDebug() << "TcpThreadManager::connectToServer - 连接结果:" << result;
    return result;
}

qint64 TcpThreadManager::sendMessage(const TcpMessage& msg) {
    if (!m_tcpClient || !m_thread || !m_thread->isRunning()) {
        qDebug() << "错误：TcpClient未初始化或线程未运行";
        return -1;
    }

    qint64 result = -1;
    QMetaObject::invokeMethod(m_tcpClient, [this, msg, &result]() {
        result = m_tcpClient->sendMessage(msg);
    }, Qt::BlockingQueuedConnection);

    return result;
}

void TcpThreadManager::setHeartbeatInterval(int interval) {
    if (!m_tcpClient || !m_thread || !m_thread->isRunning()) {
        qDebug() << "错误：TcpClient未初始化或线程未运行";
        return;
    }

    QMetaObject::invokeMethod(m_tcpClient, "setHeartbeatInterval",
                              Qt::QueuedConnection,
                              Q_ARG(int, interval));
}

void TcpThreadManager::debugThreadInfo() {
    if (!m_tcpClient || !m_thread || !m_thread->isRunning()) {
        qDebug() << "错误：TcpClient未初始化或线程未运行";
        return;
    }

    QMetaObject::invokeMethod(m_tcpClient, "debugThreadInfo", Qt::QueuedConnection);
}
