#include "loginview.h"
#include "ui_loginview.h"
#include "idatabase.h"

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{
    ui->setupUi(this);
}

LoginView::~LoginView()
{
    delete ui;
}

void LoginView::on_btsignin_clicked()
{
    QString status=idatabase::getInstance().userlogin(ui->lineEdit->text(),ui->lineEdit_2->text());

    if(status=="login ok")
    emit loginsuccess();
}

