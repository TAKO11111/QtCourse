#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QListView>
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "contactmodel.h"
#include "messagemodel.h"
#include "tcpclient.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void init(const QString& username);

private slots:
    void onSendMessageClicked();
    void onMessageReceived(const TcpMessage& msg);
    void onConnectStateChanged(bool connected);
    void onContactItemClicked(QTreeWidgetItem* item, int column);

private:
    void initUI();
    void initNetwork();
    void initModel();
    void loadContactsToTreeWidget();
    void delayedConnect();

    QTreeWidget* m_contactTree;
    QListView* m_messageListView;
    QLineEdit* m_messageInput;
    QPushButton* m_sendBtn;
    QSplitter* m_mainSplitter;
    QVBoxLayout* m_chatLayout;
    QHBoxLayout* m_inputLayout;

    ContactModel* m_contactModel;
    MessageModel* m_messageModel;
    TcpClient* m_tcpClient;

    QString m_currentUser;
    QString m_currentChatTarget;
    bool m_connectionInProgress;
};

#endif // MAINWINDOW_H
