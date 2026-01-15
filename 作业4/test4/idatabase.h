#ifndef IDATABASE_H
#define IDATABASE_H

#include <QObject>
#include <QSqlDatabase> // 替换<Qtsql>，规范头文件包含
#include <QSqlQuery>
#include <QSqlError>
#include <QDataWidgetMapper>
#include <QSqlTableModel>
#include <QItemSelectionModel>

class idatabase : public QObject
{
    Q_OBJECT
public:
    static idatabase& getInstance()
    {
        static idatabase instance;
        return instance;
    }
    QString userlogin(QString username, QString password);
    int addnewpatient();


private:
    explicit idatabase(QObject *parent = nullptr);
    idatabase(idatabase const&) = delete;
    void operator=(idatabase const&) = delete;

    QSqlDatabase database;
    void inindatabase();



signals:

public:
    QSqlTableModel *patienttabmodel;

    QItemSelectionModel *patienttheselection;

    QDataWidgetMapper *patientdatamapper;
    bool initpatientmodel();
    bool checkpatient(QString filter);
    bool deletepatient();
    bool submitpatient();
    void revertpatient();


};

#endif // IDATABASE_H
