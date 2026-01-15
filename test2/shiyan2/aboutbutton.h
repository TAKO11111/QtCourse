#ifndef ABOUTBUTTON_H
#define ABOUTBUTTON_H

#include <QDialog>

namespace Ui {
class aboutBUTTON;
}

class aboutBUTTON : public QDialog
{
    Q_OBJECT

public:
    explicit aboutBUTTON(QWidget *parent = nullptr);
    ~aboutBUTTON();

private:
    Ui::aboutBUTTON *ui;
};

#endif // ABOUTBUTTON_H
