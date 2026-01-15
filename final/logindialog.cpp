#include "logindialog.h"
#include <QMessageBox>
#include <QFile>
#include "dbmanager.h"

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    this->setWindowTitle("IM系统 - 登录");
    this->setFixedSize(400, 300);

    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("请输入用户名");

    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("请输入密码");
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_nicknameEdit = new QLineEdit(this);
    m_nicknameEdit->setPlaceholderText("注册时填写昵称");
    m_nicknameEdit->setVisible(false);

    m_loginBtn = new QPushButton("登录", this);
    m_registerBtn = new QPushButton("注册", this);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_usernameEdit);
    mainLayout->addWidget(m_passwordEdit);
    mainLayout->addWidget(m_nicknameEdit);
    mainLayout->addSpacing(20);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_loginBtn);
    btnLayout->addWidget(m_registerBtn);
    mainLayout->addLayout(btnLayout);

    connect(m_loginBtn, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(m_registerBtn, &QPushButton::clicked, this, &LoginDialog::onRegisterClicked);

    QFile styleFile("./resources/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        this->setStyleSheet(QLatin1String(styleFile.readAll()));
        styleFile.close();
    }
}

LoginDialog::~LoginDialog() = default;

void LoginDialog::onLoginClicked() {
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "警告", "用户名和密码不能为空！");
        return;
    }

    if (DBManager::getInstance().loginVerify(username, password)) {
        emit loginSuccess(username);
    } else {
        QMessageBox::critical(this, "错误", "用户名或密码错误！");
    }
}

void LoginDialog::onRegisterClicked() {
    if (m_nicknameEdit->isVisible()) {
        QString username = m_usernameEdit->text().trimmed();
        QString password = m_passwordEdit->text().trimmed();
        QString nickname = m_nicknameEdit->text().trimmed();

        if (username.isEmpty() || password.isEmpty() || nickname.isEmpty()) {
            QMessageBox::warning(this, "警告", "所有字段不能为空！");
            return;
        }

        if (DBManager::getInstance().registerUser(username, password, nickname)) {
            QMessageBox::information(this, "成功", "注册成功，请登录！");
            m_nicknameEdit->setVisible(false);
            m_registerBtn->setText("注册");
        } else {
            QMessageBox::critical(this, "错误", "注册失败（用户名已存在）！");
        }
    } else {
        m_nicknameEdit->setVisible(true);
        m_registerBtn->setText("提交注册");
        this->setWindowTitle("IM系统 - 注册");
    }
}
