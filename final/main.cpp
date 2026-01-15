#include "mainwindow.h"
#include <QApplication>
#include "logindialog.h"
#include "dbmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 初始化数据库
    DBManager::getInstance().initDB();

    // 显示登录窗口
    LoginDialog loginDialog;
    if (loginDialog.exec() != QDialog::Accepted) {
        return 0; // 用户取消登录，退出程序
    }

    // 显示主窗口
    MainWindow w;
    w.init(loginDialog.getUsername());
    w.show();

    return a.exec();
}
