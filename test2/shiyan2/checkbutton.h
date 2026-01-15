#ifndef CHECKBUTTON_H
#define CHECKBUTTON_H

#include <QDialog>
#include "QPlainTextEdit"

namespace Ui {
class checkBUTTON;
}

class checkBUTTON : public QDialog
{
    Q_OBJECT

public:
    explicit checkBUTTON(QWidget *parent = nullptr,QPlainTextEdit *TextEdit=nullptr);
    ~checkBUTTON();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::checkBUTTON *ui;

    QPlainTextEdit *ptextEdit;
};

#endif // CHECKBUTTON_H
