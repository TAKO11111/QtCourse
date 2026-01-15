#include "checkbutton.h"
#include "ui_checkbutton.h"
#include "QMessageBox"

checkBUTTON::checkBUTTON(QWidget *parent,QPlainTextEdit *TextEdit)
    : QDialog(parent)
    , ui(new Ui::checkBUTTON)
{
    ui->setupUi(this);

    ptextEdit=TextEdit;
    ui->radioButton_2->setChecked(true);
}

checkBUTTON::~checkBUTTON()
{
    delete ui;
}

void checkBUTTON::on_pushButton_clicked()
{
    QString target=ui->lineEdit->text();

    if(target==""||ptextEdit==nullptr){
        return;
    }

    QString text=ptextEdit->toPlainText();
    QTextCursor c=ptextEdit->textCursor();
    int index=-1;

    if(ui->radioButton_2->isChecked()){

        index=text.indexOf(target,c.position(),ui->checkBox->isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive);
        if(index>=0){
            c.setPosition(index);
            c.setPosition(index+target.length(),QTextCursor::KeepAnchor);

            ptextEdit->setTextCursor(c);
        }
    }else  if(ui->radioButton->isChecked()){
        index=text.lastIndexOf(target,c.position()-text.length()-1);
        if(index>=0){
            c.setPosition(index+target.length());
            c.setPosition(index,QTextCursor::KeepAnchor);

            ptextEdit->setTextCursor(c);
        }
    }
    if(index<0){
        QMessageBox msg(this);
        msg.setWindowTitle("报告");
        msg.setText(QString("找不到"+target));
        msg.setWindowFlag(Qt::Drawer);
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}


void checkBUTTON::on_pushButton_2_clicked()
{
    accept();
}

