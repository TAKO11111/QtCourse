#include "repacesbutton.h"
#include "ui_repacesbutton.h"
#include "QMessageBox"


repacesBUTTON::repacesBUTTON(QWidget *parent,QPlainTextEdit*TextEdit)
    : QDialog(parent)
    , ui(new Ui::repacesBUTTON)
{
    ui->setupUi(this);
    pTextEdit=TextEdit;
    ui->radioButton_2->setChecked(true);
}

repacesBUTTON::~repacesBUTTON()
{
    delete ui;
}

void repacesBUTTON::on_pushButton_2_clicked()
{
    QString target=ui->lineEdit->text();
    QString to=ui->lineEdit_2->text();

    if((pTextEdit!=nullptr)&&(target!="")&&(to!="")){
        QString selText =pTextEdit->textCursor().selectedText();

        if(selText==target){
            pTextEdit->insertPlainText(to);
        }
        on_pushButton_clicked();
    }

}


void repacesBUTTON::on_pushButton_3_clicked()
{
    QString target=ui->lineEdit->text();
    QString to=ui->lineEdit_2->text();

    if((pTextEdit!=nullptr)&&(target!="")&&(to!="")){
        QString text=pTextEdit->toPlainText();
        text.replace(target,to,ui->checkBox->isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive);
        pTextEdit->clear();
        pTextEdit->insertPlainText(text);
    }
}


void repacesBUTTON::on_pushButton_4_clicked()
{
    accept();
}


void repacesBUTTON::on_pushButton_clicked()
{
    QString target=ui->lineEdit->text();

    if(target==""||pTextEdit==nullptr){
        return;
    }

    QString text=pTextEdit->toPlainText();
    QTextCursor c=pTextEdit->textCursor();
    int index=-1;

    if(ui->radioButton_2->isChecked()){

        index=text.indexOf(target,c.position(),ui->checkBox->isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive);
        if(index>=0){
            c.setPosition(index);
            c.setPosition(index+target.length(),QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
        }
    }else  if(ui->radioButton->isChecked()){
        index=text.lastIndexOf(target,c.position()-text.length()-1);
        if(index>=0){
            c.setPosition(index+target.length());
            c.setPosition(index,QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
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

