#include "idatabase.h"
#include <QSqlError>
#include <QFile>
#include <QDebug>    // 加调试头文件（如果没加的话）

void idatabase::inindatabase()
{
    database=QSqlDatabase::addDatabase("QSQLITE");
    QString afile="D:/qt/lab4.db";
    database.setDatabaseName(afile);


    if(database.open()){
        qDebug()<<"open database is ok";
    }else
        qDebug()<<"failed to open  database";
}

bool idatabase::initpatientmodel()
{
    patienttabmodel =new QSqlTableModel(this,database);
    patienttabmodel->setTable("patient");
    patienttabmodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    patienttabmodel->setSort(patienttabmodel->fieldIndex("name"),Qt::AscendingOrder);
    if(!(patienttabmodel->select()))
        return false;
    patienttheselection =new QItemSelectionModel(patienttabmodel);
    return true;
}

bool idatabase::checkpatient(QString filter)
{
    patienttabmodel->setFilter(filter);
    return patienttabmodel->select();
}

bool idatabase::deletepatient()
{
    QModelIndex curindex =patienttheselection->currentIndex();
    patienttabmodel->removeRow(curindex.row());
    patienttabmodel->submitAll();
    patienttabmodel->select();
}

bool idatabase::submitpatient()
{
    patienttabmodel->submitAll();
}

void idatabase::revertpatient()
{
    patienttabmodel->revertAll();
}



QString idatabase::userlogin(QString username, QString password)
{
    QSqlQuery query;
    query.prepare("select username,password from user where username = :USER");
    query.bindValue(":USER",username);
    query.exec();
    if(query.first()&&query.value("username").isValid()){
        QString passwd =query.value("password").toString();
        if(passwd==password){
            return "login ok";
        }
        else{
            return "wrong password";
        }
    }else{
        qDebug()<<"no such user";
        return "wrong username";
    }
}

int idatabase::addnewpatient()
{
    patienttabmodel->insertRow(patienttabmodel->rowCount(),QModelIndex());

    QModelIndex curindex=patienttabmodel->index(patienttabmodel->rowCount()-1,1);
    return curindex.row();
}

idatabase::idatabase(QObject *parent)
    : QObject{parent}
{
    inindatabase(); // 构造函数里初始化数据库，正确
}
