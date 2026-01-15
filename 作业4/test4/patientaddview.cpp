#include "patientaddview.h"
#include "ui_patientaddview.h"

patientaddview::patientaddview(QWidget *parent,int index)
    : QWidget(parent)
    , ui(new Ui::patientaddview)
{
    ui->setupUi(this);
}

patientaddview::~patientaddview()
{
    delete ui;
}
