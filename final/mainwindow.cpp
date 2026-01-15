#include "mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QStandardItemModel>
#include "dbmanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_contactModel(new ContactModel(this)),
    m_messageModel(new MessageModel(this)),
    m_tcpClient(TcpThreadManager::getInstance().getTcpClient())
{
    initUI();
    initNetwork();
    initModel();
}

MainWindow::~MainWindow() {}

void MainWindow::init(const QString& username) {
    m_currentUser = username;
    this->setWindowTitle(QString("IM系统 - %1").arg(username));

    // 加载联系人
    m_contactModel->loadContacts(username);
    // 连接到服务器（测试用，替换为你的服务器IP和端口）
    m_tcpClient->connectToServer("127.0.0.1", 8888);

    // 加载样式表
    QFile styleFile("./resources/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = QLatin1String(styleFile.readAll());
        this->setStyleSheet(style);
        styleFile.close();
    }
}

void MainWindow::initUI() {
    // 设置窗口属性
    this->setObjectName("MainWindow");
    this->setMinimumSize(800, 600);

    // 创建控件
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

    // 布局
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
    m_mainSplitter->setSizes({200, 600}); // 左侧200px，右侧600px

    this->setCentralWidget(m_mainSplitter);

    // 绑定信号槽
    connect(m_sendBtn, &QPushButton::clicked, this, &MainWindow::onSendMessageClicked);
    connect(m_contactTree, &QTreeWidget::clicked, this, &MainWindow::onContactClicked);
    connect(m_tcpClient, &TcpClient::messageReceived, this, &MainWindow::onMessageReceived);
    connect(m_tcpClient, &TcpClient::connectStateChanged, this, &MainWindow::onConnectStateChanged);
}

void MainWindow::initNetwork() {
    // 设置心跳间隔（30秒）
    m_tcpClient->setHeartbeatInterval(30000);
}

void MainWindow::initModel() {
    // 绑定联系人模型到树控件（简化版，实际可自定义Delegate）
    // 此处先手动加载联系人到TreeWidget（后续可替换为QTreeView+ContactModel）
    m_contactTree->clear();
}

void MainWindow::onSendMessageClicked() {
    QString content = m_messageInput->text().trimmed();
    if (content.isEmpty() || m_currentChatTarget.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择聊天对象并输入消息！");
        return;
    }

    // 构造单聊消息
    TcpMessage msg(MessageType::SingleChat, m_currentUser, m_currentChatTarget, content);
    // 发送网络消息
    m_tcpClient->sendMessage(msg);
    // 添加到本地消息模型
    MessageItem item;
    item.content = content;
    item.time = msg.time();
    item.type = MessageRoleType::Send;
    item.fromUser = m_currentUser;
    item.toUser = m_currentChatTarget;
    m_messageModel->addMessage(item);
    // 保存到数据库
    DBManager::getInstance().saveChatRecord(m_currentUser, m_currentChatTarget, content);

    // 清空输入框
    m_messageInput->clear();
}

void MainWindow::onMessageReceived(const TcpMessage& msg) {
    // 只处理单聊消息
    if (msg.type() != MessageType::SingleChat) {
        return;
    }

    // 检查是否是发给当前用户的消息
    if (msg.to() == m_currentUser) {
        // 添加到本地消息模型
        MessageItem item;
        item.content = msg.content();
        item.time = msg.time();
        item.type = MessageRoleType::Receive;
        item.fromUser = msg.from();
        item.toUser = msg.to();
        m_messageModel->addMessage(item);
        // 保存到数据库
        DBManager::getInstance().saveChatRecord(msg.from(), msg.to(), msg.content());
    }
}

void MainWindow::onConnectStateChanged(bool connected) {
    if (connected) {
        QMessageBox::information(this, "提示", "已连接到服务器！");
    } else {
        QMessageBox::warning(this, "提示", "与服务器断开连接，正在重连...");
    }
}

void MainWindow::onContactClicked(const QModelIndex& index) {
    // 获取选中的联系人（简化版，实际需解析ContactModel）
    QTreeWidgetItem* item = m_contactTree->currentItem();
    if (item && !item->parent()) {
        // 点击的是分组，不处理
        return;
    }

    if (item) {
        m_currentChatTarget = item->text(0);
        // 加载该联系人的历史消息
        m_messageModel->loadHistoryMessages(m_currentUser, m_currentChatTarget);
    }
}
