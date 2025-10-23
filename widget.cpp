#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->btnNum0,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum1,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum2,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum3,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum4,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum5,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum6,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum7,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum8,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum9,SIGNAL(clicked()),this,SLOT(btnNumClicked()));

    connect(ui->btnjia,SIGNAL(clicked()),this,SLOT(btnoperandsClick()));
    connect(ui->btnjian,SIGNAL(clicked()),this,SLOT(btnoperandsClick()));
    connect(ui->btncheng,SIGNAL(clicked()),this,SLOT(btnoperandsClick()));
    connect(ui->btnchu,SIGNAL(clicked()),this,SLOT(btnoperandsClick()));
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::calulation(bool &ok)
{

}

void Widget::btnNumClicked()
{   QString digit=qobject_cast<QPushButton*>(sender())->text();
    if(digit=="0"&&operand=="0")
        digit="";
    if(operand=="0"&&digit!="0")
        operand="";
    operand+=digit;
    ui->dispaly->setText(operand);
}


void Widget::on_btnpoint_clicked()
{
    if(!operand.contains("."))
        operand+=qobject_cast<QPushButton*>(sender())->text();
    ui->dispaly->setText(operand);
}


void Widget::on_btnDivide_clicked()
{
    operand=operand.left(operand.length()-1);
    ui->dispaly->setText(operand);
}


void Widget::on_btnClear_clicked()
{
    operand.clear();
    ui->dispaly->setText(operand);
}

void Widget::btnoperandsClick()
{
    opcode=qobject_cast<QPushButton*>(sender())->text();

    if(operand!=""){
        operands.push_back(operand);
        operand="";

        opcodes.push_back(opcode);
    }
    QString result=calulation();
    ui->dispaly->setText(result);
}

void Widget::on_btndengyu_clicked()
{
    if(operand!=""){
        operands.push_back(operand);
        operand="";
    }
    QString result=calulation();
    ui->dispaly->setText(result);
}

