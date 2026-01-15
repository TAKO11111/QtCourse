#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>

namespace Ui {
class LoginView;
}

class LoginView : public QWidget
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();

private slots:
    void on_btsignin_clicked();

signals:
    void loginsuccess();
    void loginfailed();

private:
    Ui::LoginView *ui;
};

#endif // LOGINVIEW_H
