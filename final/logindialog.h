#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "dbmanager.h"

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog() = default;

    // 获取登录成功后的用户名
    QString getUsername() const { return m_usernameEdit->text(); }

signals:
    // 登录成功信号
    void loginSuccess(const QString& username);
    // 注册信号
    void registerUser(const QString& username, const QString& password, const QString& nickname);

private slots:
    // 登录按钮点击事件
    void onLoginClicked();
    // 注册按钮点击事件
    void onRegisterClicked();

private:
    // 控件
    QLineEdit* m_usernameEdit;    // 用户名输入框
    QLineEdit* m_passwordEdit;    // 密码输入框
    QLineEdit* m_nicknameEdit;    // 昵称输入框（注册用）
    QPushButton* m_loginBtn;      // 登录按钮
    QPushButton* m_registerBtn;   // 注册按钮

    // 布局
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_btnLayout;

    // 初始化UI
    void initUI();
};

#endif // LOGINDIALOG_H
