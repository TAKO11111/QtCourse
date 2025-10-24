#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStack>
#include <QKeyEvent>
#include <QMap>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;  // UI 类名与核心类一致
}
QT_END_NAMESPACE

class Widget : public QWidget  // 核心类为 Widget，继承自 QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QString operand;               // 当前输入的操作数
    QStack<QString> operands;      // 操作数栈
    QStack<QString> opcodes;       // 运算符栈
    QMap<int, QPushButton*> digitBTNs;  // 数字按钮与键盘按键的映射

    QString calculation(bool *ok = nullptr);  // 计算逻辑

private slots:
    void btnNumClicked();         // 数字按钮点击
    void binaryOperatorClicked(); // 双目运算符点击
    void btnUnaryOperatorClicked(); // 单目运算符点击
    void on_btnPeriod_clicked();   // 小数点按钮
    void on_btnDel_clicked();      // 删除按钮
    void on_btnClear_clicked();    // 清除当前输入
    void on_btnEqual_clicked();    // 等号按钮
    void on_btnClearAll_clicked(); // 清除所有
    void on_btnOpposite_clicked(); // 正负号按钮

protected:
    void keyPressEvent(QKeyEvent *event) override;  // 重写键盘事件

private:
    Ui::Widget *ui;  // UI 指针与核心类一致
};

#endif // WIDGET_H
