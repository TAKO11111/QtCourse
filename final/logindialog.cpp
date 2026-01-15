#include "logindialog.h"
#include <QMessageBox>
#include <QFile>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    initUI();

    // 加载样式表
    QFile styleFile("./resources/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = QLatin1String(styleFile.readAll());
        this->setStyleSheet(style);
        styleFile.close();
    }

    // 绑定信号槽
    connect(m_loginBtn, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(m_registerBtn, &QPushButton::clicked, this, &LoginDialog::onRegisterClicked);
}

void LoginDialog::initUI() {
    // 设置窗口属性
    this->setWindowTitle("IM系统 - 登录");
    this->setFixedSize(350, 300);

    // 创建控件
    QLabel* titleLabel = new QLabel("即时通讯系统");
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel* usernameLabel = new QLabel("用户名：");
    m_usernameEdit = new QLineEdit();
    m_usernameEdit->setPlaceholderText("请输入用户名");

    QLabel* passwordLabel = new QLabel("密  码：");
    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setPlaceholderText("请输入密码");
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    QLabel* nicknameLabel = new QLabel("昵  称：");
    m_nicknameEdit = new QLineEdit();
    m_nicknameEdit->setPlaceholderText("注册时填写昵称");

    m_loginBtn = new QPushButton("登录");
    m_loginBtn->setObjectName("LoginBtn");
    m_registerBtn = new QPushButton("注册");
    m_registerBtn->setObjectName("RegisterBtn");

    // 布局
    m_mainLayout = new QVBoxLayout(this);
    m_btnLayout = new QHBoxLayout();

    m_btnLayout->addWidget(m_loginBtn);
    m_btnLayout->addWidget(m_registerBtn);
    m_btnLayout->setSpacing(20);

    m_mainLayout->addWidget(titleLabel);
    m_mainLayout->addSpacing(20);
    m_mainLayout->addWidget(usernameLabel);
    m_mainLayout->addWidget(m_usernameEdit);
    m_mainLayout->addWidget(passwordLabel);
    m_mainLayout->addWidget(m_passwordEdit);
    m_mainLayout->addWidget(nicknameLabel);
    m_mainLayout->addWidget(m_nicknameEdit);
    m_mainLayout->addSpacing(20);
    m_mainLayout->addLayout(m_btnLayout);

    m_mainLayout->setContentsMargins(40, 20, 40, 20);
    m_mainLayout->setSpacing(10);
}

void LoginDialog::onLoginClicked() {
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text().trimmed();

    // 校验输入
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "警告", "用户名和密码不能为空！");
        return;
    }

    // 数据库验证登录
    bool loginOk = DBManager::getInstance().loginVerify(username, password);
    if (loginOk) {
        QMessageBox::information(this, "成功", "登录成功！");
        emit loginSuccess(username);
        this->accept(); // 关闭登录窗口
    } else {
        QMessageBox::critical(this, "错误", "用户名或密码错误！");
    }
}

void LoginDialog::onRegisterClicked() {
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text().trimmed();
    QString nickname = m_nicknameEdit->text().trimmed();

    // 校验输入
    if (username.isEmpty() || password.isEmpty() || nickname.isEmpty()) {
        QMessageBox::warning(this, "警告", "用户名、密码、昵称不能为空！");
        return;
    }

    // 数据库注册用户
    bool registerOk = DBManager::getInstance().registerUser(username, password, nickname);
    if (registerOk) {
        QMessageBox::information(this, "成功", "注册成功！请登录");
        m_nicknameEdit->clear();
    } else {
        QMessageBox::critical(this, "错误", "注册失败！用户名已存在");
    }
}
