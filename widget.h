#ifndef WIDGET_H
#define WIDGET_H
#include <QStack>

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QString operand;
    QString opcode;
    QStack<QString> operands;
    QStack<QString> opcodes;

    QString  calulation(bool &ok=NULL);

private slots:
    void btnNumClicked();
    void btnoperandsClick();


    void on_btnpoint_clicked();

    void on_btnDivide_clicked();

    void on_btnClear_clicked();

    void on_btndengyu_clicked();

private:
    Ui::Widget *ui;
};


#endif // WIDGET_H
