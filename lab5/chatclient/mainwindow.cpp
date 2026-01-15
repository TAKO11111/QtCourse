#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QJsonValue>
#include <QJsonObject>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->loginpage);
     m_chatclient =new chatclient(this);
    connect(m_chatclient,&chatclient::connected,this,&MainWindow::connecttoserver);
    // connect(m_chatclient,&chatclient::messagereceived,this,&MainWindow::messagereceived);
    connect(m_chatclient,&chatclient::jsonreceived,this,&MainWindow::jsonreceived);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginbt_clicked()
{
    m_chatclient->connecttoserver(QHostAddress(ui->serveredit->text()),1967);
}


void MainWindow::on_saybt_clicked()
{
    if(!ui->lineEdit->text().isEmpty())
        m_chatclient->sendmessage(ui->lineEdit->text());

    for(auto atem :ui->listWidget->findItems(ui->userdeit->text(),Qt::MatchExactly)){
        qDebug("remove");
        ui->listWidget->removeItemWidget(atem);
        delete atem;
    }
}


void MainWindow::on_logoutbt_clicked()
{
    m_chatclient->disconnectfromhost();
    ui->stackedWidget->setCurrentWidget(ui->loginpage);
    m_chatclient->sendmessage(ui->userdeit->text(),"login");
}

void MainWindow::connecttoserver()
{
    ui->stackedWidget->setCurrentWidget(ui->chatpage);
    m_chatclient->sendmessage(ui->userdeit->text(),"login");
}

void MainWindow::messagereceived(const QString &sender,const QString &text)
{
    ui->roomtextdit->append(QString("%1:%2").arg(sender).arg(text));
}

void MainWindow::jsonreceived(const QJsonObject &docobj)
{
    const QJsonValue typeval=docobj.value("type");
    if(typeval.isNull()||!typeval.isString())
        return;
    if(typeval.toString().compare("message",Qt::CaseInsensitive)==0){
        const QJsonValue textval=docobj.value("text");
        const QJsonValue senderVal =docobj.value("sender");
        if(textval.isNull()||!textval.isString())
            return;
        if(senderVal.isNull()||!senderVal.isString())
            return;

        messagereceived(senderVal.toString(),textval.toString());
    }
    else if(typeval.toString().compare("newuser",Qt::CaseInsensitive)==0){
        const QJsonValue usernameval=docobj.value("username");
        if(usernameval.isNull()||!usernameval.isString())
            return;
        usernamejoin(usernameval.toString());

    }    else if(typeval.toString().compare("userdisconnected",Qt::CaseInsensitive)==0){
        const QJsonValue usernameval=docobj.value("username");
        if(usernameval.isNull()||!usernameval.isString())
            return;
        usernameleft(usernameval.toString());
    }
    else if(typeval.toString().compare("userlist",Qt::CaseInsensitive)==0){
        const QJsonValue userlistval=docobj.value("userlist");
        if(userlistval.isNull()||!userlistval.isArray())
            return;
        qDebug()<<userlistval.toVariant().toStringList();
        userlistreceived(userlistval.toVariant().toStringList());
    }
}

void MainWindow::usernamejoin(const QString &user)
{
    ui->listWidget->addItem(user);
}

void MainWindow::usernameleft(const QString &user)
{
    for(auto atem :ui->listWidget->findItems(user,Qt::MatchExactly)){
        qDebug("remove");
        ui->listWidget->removeItemWidget(atem);
        delete atem;
    }
}

void MainWindow::userlistreceived(const QStringList &list)
{
    ui->listWidget->clear();
    ui->listWidget->addItems(list);
}

