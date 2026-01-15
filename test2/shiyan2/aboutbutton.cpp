#include "aboutbutton.h"
#include "ui_aboutbutton.h"

aboutBUTTON::aboutBUTTON(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::aboutBUTTON)
{
    ui->setupUi(this);
}

aboutBUTTON::~aboutBUTTON()
{
    delete ui;
}
