#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog() override;

signals:
    void loginSuccess(const QString& username);

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    QLineEdit* m_usernameEdit;
    QLineEdit* m_passwordEdit;
    QLineEdit* m_nicknameEdit;
    QPushButton* m_loginBtn;
    QPushButton* m_registerBtn;
};

#endif // LOGINDIALOG_H
