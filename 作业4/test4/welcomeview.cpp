#include "welcomeview.h"
#include "ui_welcomeview.h"
#include <QDebug>

welcomeview::welcomeview(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::welcomeview)
{
    qDebug()<<"create welcomeview";
    ui->setupUi(this);
}

welcomeview::~welcomeview()
{
    qDebug()<<"destory welcomeview";
    delete ui;
}

void welcomeview::on_btpart_clicked()
{
    emit godepartmentview();
}


void welcomeview::on_btdo_clicked()
{
    emit godoctorview();
}


void welcomeview::on_btpa_clicked()
{
    emit gopatientview();
}

