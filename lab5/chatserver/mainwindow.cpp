#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_chatserver =new chatserver(this);

    connect(m_chatserver,&chatserver::logMessage,this,&MainWindow::logMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startstopbt_clicked()
{
    if(m_chatserver->isListening()){
        m_chatserver->stopServer();
        ui->startstopbt->setText("启动服务器");
        logMessage("服务器已停止");
    }else{
        if(!m_chatserver->listen(QHostAddress::Any,1967)){
            QMessageBox::critical(this,"错误","无法启动服务器");
            return;
        }
        logMessage("服务器已经启动");
        ui->startstopbt->setText("停止服务器");
    }
}

void MainWindow::logMessage(const QString &msg)
{
    ui->logedit->appendPlainText(msg);
}

