#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QDir>
#include "logindialog.h"
#include "dbmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "=== 客户端启动 ===";
    qDebug() << "SQL支持的驱动列表:" << QSqlDatabase::drivers();
    qDebug() << "当前程序工作目录:" << QDir::currentPath();

    if (!DBManager::getInstance().initDB()) {
        QMessageBox::critical(nullptr, "数据库错误",
                              "数据库初始化失败！\n" + DBManager::getInstance().lastError());
        return -1;
    }
    qDebug() << "数据库初始化成功";

    LoginDialog loginDialog;
    MainWindow* mainWindow = nullptr;

    QObject::connect(&loginDialog, &LoginDialog::loginSuccess, [&](const QString& username) {
        qDebug() << "用户登录成功:" << username;
        mainWindow = new MainWindow();
        mainWindow->init(username);
        mainWindow->show();
        loginDialog.close();
    });

    qDebug() << "显示登录窗口";
    loginDialog.show();

    int ret = a.exec();
    delete mainWindow;
    qDebug() << "=== 客户端退出 ===";
    return ret;
}
