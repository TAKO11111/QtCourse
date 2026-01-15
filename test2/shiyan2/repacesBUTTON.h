#ifndef REPACESBUTTON_H
#define REPACESBUTTON_H

#include <QDialog>
#include "QPlainTextEdit"

namespace Ui {
class repacesBUTTON;
}

class repacesBUTTON : public QDialog
{
    Q_OBJECT

public:
    explicit repacesBUTTON(QWidget *parent = nullptr,QPlainTextEdit*TextEdit=nullptr);
    ~repacesBUTTON();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

private:
    Ui::repacesBUTTON *ui;
    QPlainTextEdit *pTextEdit;
};

#endif // REPACESBUTTON_H
