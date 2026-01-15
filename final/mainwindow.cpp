#include "mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include "dbmanager.h"
#include "tcpthreadmanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_contactModel(new ContactModel(this)),
    m_messageModel(new MessageModel(this)),
    m_tcpClient(nullptr),
    m_connectionInProgress(false)
{
    qDebug() << "MainWindow构造完成";
    initUI();
    initModel();
}

MainWindow::~MainWindow() {
    qDebug() << "MainWindow析构";
}

void MainWindow::init(const QString& username) {
    m_currentUser = username;
    this->setWindowTitle(QString("IM系统 - %1").arg(username));

    qDebug() << "MainWindow初始化，当前用户:" << username;

    // 先加载联系人
    m_contactModel->loadContacts(username);
    loadContactsToTreeWidget();

    // 获取TcpThreadManager实例
    TcpThreadManager& manager = TcpThreadManager::getInstance();

    // 连接信号
    connect(&manager, &TcpThreadManager::messageReceived,
            this, &MainWindow::onMessageReceived, Qt::QueuedConnection);
    connect(&manager, &TcpThreadManager::connectStateChanged,
            this, &MainWindow::onConnectStateChanged, Qt::QueuedConnection);
    connect(&manager, &TcpThreadManager::errorOccurred, this, [=](const QString& msg) {
        QMessageBox::warning(this, "网络错误", msg);
    }, Qt::QueuedConnection);

    // 设置心跳间隔
    manager.setHeartbeatInterval(30000);

    // 延迟连接服务器，确保GUI完全初始化
    QTimer::singleShot(500, this, &MainWindow::delayedConnect);

    // 加载样式
    QFile styleFile("./resources/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        this->setStyleSheet(QLatin1String(styleFile.readAll()));
        styleFile.close();
        qDebug() << "样式文件加载成功";
    } else {
        qDebug() << "样式文件不存在，使用默认样式";
    }
}

void MainWindow::delayedConnect() {
    if (m_connectionInProgress) return;

    m_connectionInProgress = true;
    qDebug() << "开始连接服务器...";

    TcpThreadManager& manager = TcpThreadManager::getInstance();
    bool connected = manager.connectToServer("127.0.0.1", 8888);

    if (!connected) {
        QMessageBox::warning(this, "提示",
                             "连接服务器失败！\n请确保服务器程序已启动并监听8888端口。");
    }

    m_connectionInProgress = false;
}
void MainWindow::initUI() {
    qDebug() << "初始化UI...";

    this->setObjectName("MainWindow");
    this->setMinimumSize(800, 600);

    m_contactTree = new QTreeWidget(this);
    m_contactTree->setObjectName("ContactTree");
    m_contactTree->setHeaderLabel("联系人");

    m_messageListView = new QListView(this);
    m_messageListView->setObjectName("MessageListView");
    m_messageListView->setModel(m_messageModel);

    m_messageInput = new QLineEdit(this);
    m_messageInput->setObjectName("MessageInput");
    m_messageInput->setPlaceholderText("请输入消息...");

    m_sendBtn = new QPushButton("发送", this);
    m_sendBtn->setObjectName("SendBtn");

    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    m_chatLayout = new QVBoxLayout();
    m_inputLayout = new QHBoxLayout();

    m_inputLayout->addWidget(m_messageInput);
    m_inputLayout->addWidget(m_sendBtn);
    m_inputLayout->setContentsMargins(0, 10, 0, 10);
    m_inputLayout->setSpacing(10);

    m_chatLayout->addWidget(m_messageListView);
    m_chatLayout->addLayout(m_inputLayout);
    m_chatLayout->setContentsMargins(10, 10, 10, 10);

    QWidget* chatWidget = new QWidget(this);
    chatWidget->setLayout(m_chatLayout);

    m_mainSplitter->addWidget(m_contactTree);
    m_mainSplitter->addWidget(chatWidget);
    m_mainSplitter->setSizes({200, 600});

    this->setCentralWidget(m_mainSplitter);

    connect(m_sendBtn, &QPushButton::clicked, this, &MainWindow::onSendMessageClicked);
    connect(m_contactTree, &QTreeWidget::itemClicked, this, &MainWindow::onContactItemClicked);

    qDebug() << "UI初始化完成";
}

void MainWindow::initNetwork() {
    qDebug() << "初始化网络设置...";
    TcpThreadManager& manager = TcpThreadManager::getInstance();
    manager.setHeartbeatInterval(30000);
    qDebug() << "心跳间隔设置为30秒";
}

void MainWindow::initModel() {
    qDebug() << "初始化模型...";
    m_contactTree->clear();
    qDebug() << "模型初始化完成";
}

void MainWindow::loadContactsToTreeWidget() {
    qDebug() << "加载联系人到树控件...";
    m_contactTree->clear();
    const QList<ContactGroup>& groups = m_contactModel->getGroups();

    qDebug() << "联系人分组数量:" << groups.size();
    for (const auto& group : groups) {
        QTreeWidgetItem* groupItem = new QTreeWidgetItem(m_contactTree);
        groupItem->setText(0, group.groupName);
        qDebug() << "分组:" << group.groupName << "，联系人数量:" << group.contactList.size();

        for (const auto& contact : group.contactList) {
            QTreeWidgetItem* contactItem = new QTreeWidgetItem(groupItem);
            contactItem->setText(0, contact);
        }
    }
    qDebug() << "联系人加载完成";
}

void MainWindow::onSendMessageClicked() {
    QString content = m_messageInput->text().trimmed();
    if (content.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入消息内容！");
        return;
    }

    if (m_currentChatTarget.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择聊天对象！");
        return;
    }

    TcpThreadManager& manager = TcpThreadManager::getInstance();
    TcpMessage msg(MessageType::SingleChat, m_currentUser, m_currentChatTarget, content);
    qint64 result = manager.sendMessage(msg);

    if (result < 0) {
        QMessageBox::warning(this, "提示", "消息发送失败！");
        return;
    }

    MessageItem item;
    item.content = content;
    item.time = QDateTime::currentDateTime();
    item.type = MessageRoleType::Send;
    item.fromUser = m_currentUser;
    item.toUser = m_currentChatTarget;
    m_messageModel->addMessage(item);

    DBManager::getInstance().saveChatRecord(m_currentUser, m_currentChatTarget, content);

    m_messageInput->clear();
    qDebug() << "消息发送成功，内容:" << content << "，目标:" << m_currentChatTarget;
}

void MainWindow::onMessageReceived(const TcpMessage& msg) {
    qDebug() << "MainWindow::onMessageReceived - 接收到消息，类型:" << static_cast<int>(msg.type())
              << "，来自:" << msg.from() << "，内容:" << msg.content();

    if (msg.type() != MessageType::SingleChat) {
        qDebug() << "非单聊消息，跳过处理";
        return;
    }

    if (msg.to() == m_currentUser) {
        qDebug() << "消息是发给当前用户的";

        // 如果当前没有聊天目标，或者消息来自当前聊天目标
        if (m_currentChatTarget.isEmpty() || m_currentChatTarget == msg.from()) {
            m_currentChatTarget = msg.from();
            qDebug() << "设置当前聊天目标为:" << m_currentChatTarget;

            // 加载历史消息
            m_messageModel->loadHistoryMessages(m_currentUser, m_currentChatTarget);
        }

        MessageItem item;
        item.content = msg.content();
        item.time = msg.time();
        item.type = MessageRoleType::Receive;
        item.fromUser = msg.from();
        item.toUser = msg.to();
        m_messageModel->addMessage(item);

        DBManager::getInstance().saveChatRecord(msg.from(), msg.to(), msg.content());

        qDebug() << "消息已添加到界面并保存到数据库";
    } else {
        qDebug() << "消息不是发给当前用户的，跳过";
    }
}

void MainWindow::onConnectStateChanged(bool connected) {
    qDebug() << "MainWindow::onConnectStateChanged - 连接状态:" << (connected ? "已连接" : "已断开");

    if (connected) {
        QMessageBox::information(this, "提示", "已连接到服务器！");
    } else {
        QMessageBox::warning(this, "提示", "与服务器断开连接！");
    }
}

void MainWindow::onContactItemClicked(QTreeWidgetItem* item, int column) {
    Q_UNUSED(column);

    if (!item) {
        qDebug() << "点击了空项目";
        m_currentChatTarget = "";
        return;
    }

    // 如果点击的是分组项（没有父项）
    if (!item->parent()) {
        qDebug() << "点击了分组:" << item->text(0);
        m_currentChatTarget = "";
        m_messageModel->clearMessages();
        return;
    }

    // 点击的是联系人项
    QString contactName = item->text(0);
    QString groupName = item->parent()->text(0);

    qDebug() << "点击了联系人:" << contactName << "，分组:" << groupName;

    m_currentChatTarget = contactName;
    m_messageModel->loadHistoryMessages(m_currentUser, m_currentChatTarget);

    qDebug() << "当前聊天目标设置为:" << m_currentChatTarget;
}
