#include "masterview.h"
#include "ui_masterview.h"
#include <QDebug>
#include "idatabase.h"

MasterView::MasterView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MasterView)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);

    gologinview();

    idatabase::getInstance();

}

MasterView::~MasterView()
{
    delete ui;
}

void MasterView::gologinview()
{
    qDebug()<<"gologinview";
    loginview =new LoginView(this);

    pushview(loginview);

    connect(loginview, SIGNAL(loginsuccess()), this, SLOT(gowelcomeview()));
}

void MasterView::godepartmentview()
{
    qDebug()<<"godepartmentview";
    departmentview =new class departmentview(this);

    pushview(departmentview);
}

void MasterView::godoctorview()
{
    qDebug()<<"godoctorview";
    doctorview =new class doctorview(this);

    pushview(doctorview);
}

void MasterView::gopatientaddview(int rowno)
{
    qDebug()<<"gopatientaddview";
    patientaddview =new class patientaddview(this,rowno);

    pushview(patientaddview);
}

void MasterView::gopatientview()
{
    qDebug()<<"gopatientview";
    patientview =new class patientview(this);

    pushview(patientview);

    connect(patientview, SIGNAL(gopatientaddview(int)), this, SLOT(gopatientaddview(int)));
}

void MasterView::gowelcomeview()
{
    qDebug()<<"gowelcomeview";
    welcomeview =new class welcomeview(this);

    pushview(welcomeview);

    connect(welcomeview, SIGNAL(godepartmentview()), this, SLOT(godepartmentview()));
    connect(welcomeview, SIGNAL(godoctorview()), this, SLOT(godoctorview()));
    connect(welcomeview, SIGNAL(gopatientview()), this, SLOT(gopatientview()));
}

void MasterView::gopreviousview()
{
    int count=ui->stackedWidget->count();

    if(count>1){
        ui->stackedWidget->setCurrentIndex(count-2);
        ui->label->setText(ui->stackedWidget->currentWidget()->windowIconText());

        QWidget *widget=ui->stackedWidget->widget(count-1);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }
}

void MasterView::pushview(QWidget *widget)
{
    ui->stackedWidget->addWidget(widget);
    int count=ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(count-1);
    ui->label->setText(widget->windowTitle());
}

void MasterView::on_btBack_clicked()
{
    gopreviousview();
}


void MasterView::on_stackedWidget_currentChanged(int arg1)
{
    int count=ui->stackedWidget->count();
    if(count>1){
        ui->btBack->setEnabled(true);
    }
    else
        ui->btBack->setEnabled(false);

    QString title=ui->stackedWidget->currentWidget()->windowTitle();
    if(title=="欢迎"){
        ui->stackedWidget->setEnabled(true);
        ui->btBack->setEnabled(false);
        ui->btLogout->setEnabled(true);
    }
    else
        ui->btLogout->setEnabled(false);
    if(title=="登录"){
        ui->btBack->setEnabled(false);
    }
}


void MasterView::on_btLogout_clicked()
{
    gopreviousview();
}

