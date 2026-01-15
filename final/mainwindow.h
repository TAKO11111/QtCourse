#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QListView>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include "contactmodel.h"
#include "messagemodel.h"
#include "tcpclient.h"
#include "tcpmessage.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 初始化主窗口（传入登录的用户名）
    void init(const QString& username);

private slots:
    // 发送消息按钮点击事件
    void onSendMessageClicked();
    // 接收网络消息
    void onMessageReceived(const TcpMessage& msg);
    // 连接状态变化
    void onConnectStateChanged(bool connected);
    // 联系人点击事件
    void onContactClicked(const QModelIndex& index);

private:
    // 当前登录用户
    QString m_currentUser;
    // 当前选中的聊天对象
    QString m_currentChatTarget;

    // Model
    ContactModel* m_contactModel;
    MessageModel* m_messageModel;
    TcpClient* m_tcpClient;

    // UI控件
    QTreeWidget* m_contactTree;      // 联系人树
    QListView* m_messageListView;    // 消息列表
    QLineEdit* m_messageInput;       // 消息输入框
    QPushButton* m_sendBtn;          // 发送按钮

    // 布局
    QSplitter* m_mainSplitter;
    QVBoxLayout* m_chatLayout;
    QHBoxLayout* m_inputLayout;

    // 初始化UI
    void initUI();
    // 初始化网络连接
    void initNetwork();
    // 初始化Model
    void initModel();
};

#endif // MAINWINDOW_H
