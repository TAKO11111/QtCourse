#include "doctorview.h"
#include "ui_doctorview.h"

doctorview::doctorview(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::doctorview)
{
    ui->setupUi(this);
}

doctorview::~doctorview()
{
    delete ui;
}
