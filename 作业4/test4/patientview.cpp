#include "patientview.h"
#include "ui_patientview.h"
#include <idatabase.h>

patientview::patientview(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::patientview)
{
    ui->setupUi(this);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);


    idatabase &idatabase =idatabase::getInstance();
    if(idatabase.initpatientmodel()){
        ui->tableView->setModel(idatabase.patienttabmodel);
        ui->tableView->setSelectionModel(idatabase.patienttheselection);
    }
}

patientview::~patientview()
{
    delete ui;
}

void patientview::on_btadd_clicked()
{
    int currow=idatabase::getInstance().addnewpatient();
    emit gopatientaddview(currow);
}


void patientview::on_btcheck_clicked()
{
    QString filter=QString("name like '%%1%'").arg(ui->lineEdit->text());
    idatabase::getInstance().checkpatient(filter);
}


void patientview::on_btedit_clicked()
{
    QModelIndex curindex=idatabase::getInstance().patienttheselection->currentIndex();

    emit gopatientaddview(curindex.row());
}


void patientview::on_btclear_clicked()
{
    idatabase::getInstance().deletepatient();
}

