#include "departmentview.h"
#include "ui_departmentview.h"

departmentview::departmentview(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::departmentview)
{
    ui->setupUi(this);
}

departmentview::~departmentview()
{
    delete ui;
}
