#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <chatclient.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loginbt_clicked();

    void on_saybt_clicked();

    void on_logoutbt_clicked();
    void connecttoserver();
    void messagereceived(const QString &sender,const QString &text);
    void jsonreceived(const QJsonObject &docobj);
    void usernamejoin(const QString &user);
    void usernameleft(const QString &user);
    void userlistreceived(const QStringList &list);


private:
    Ui::MainWindow *ui;

    chatclient *m_chatclient;
};
#endif // MAINWINDOW_H
