#include "widget.h"
#include "ui_widget.h"  // 引用 Widget 的 UI 文件
#include <math.h>
#include <QKeyEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)  // 初始化 UI 为 Widget 对应的 UI
{
    ui->setupUi(this);
    // 数字按钮与键盘按键的映射
    digitBTNs = {
                 {Qt::Key_0, ui->btnNum0},
                 {Qt::Key_1, ui->btnNum1},
                 {Qt::Key_2, ui->btnNum2},
                 {Qt::Key_3, ui->btnNum3},
                 {Qt::Key_4, ui->btnNum4},
                 {Qt::Key_5, ui->btnNum5},
                 {Qt::Key_6, ui->btnNum6},
                 {Qt::Key_7, ui->btnNum7},
                 {Qt::Key_8, ui->btnNum8},
                 {Qt::Key_9, ui->btnNum9},
                 };
    // 批量连接数字按钮
    foreach (auto btn, digitBTNs) {
        connect(btn, &QPushButton::clicked, this, &Widget::btnNumClicked);
    }

    // 连接双目运算符按钮
    connect(ui->btnMultiple, &QPushButton::clicked, this, &Widget::binaryOperatorClicked);
    connect(ui->btnDivide, &QPushButton::clicked, this, &Widget::binaryOperatorClicked);
    connect(ui->btnPlus, &QPushButton::clicked, this, &Widget::binaryOperatorClicked);
    connect(ui->btnMinus, &QPushButton::clicked, this, &Widget::binaryOperatorClicked);

    // 连接单目运算符按钮
    connect(ui->btnPercentage, &QPushButton::clicked, this, &Widget::btnUnaryOperatorClicked);
    connect(ui->btnInverse, &QPushButton::clicked, this, &Widget::btnUnaryOperatorClicked);
    connect(ui->btnSquare, &QPushButton::clicked, this, &Widget::btnUnaryOperatorClicked);
    connect(ui->btnSqrt, &QPushButton::clicked, this, &Widget::btnUnaryOperatorClicked);
}

Widget::~Widget()
{
    delete ui;
}

// 计算逻辑实现
QString Widget::calculation(bool *ok)
{
    double result = 0;
    if (operands.size() == 2 && !opcodes.isEmpty()) {
        double operand1 = operands.front().toDouble();
        operands.pop_front();
        double operand2 = operands.front().toDouble();
        operands.pop_front();
        QString op = opcodes.front();
        opcodes.pop_front();

        if (op == "+") {
            result = operand1 + operand2;
        } else if (op == "-") {
            result = operand1 - operand2;
        } else if (op == "✖") {
            result = operand1 * operand2;
        } else if (op == "/") {
            result = operand1 / operand2;
        }
        operands.push_back(QString::number(result));

    }
    return QString::number(result);
}

// 数字按钮点击事件
void Widget::btnNumClicked()
{
    QString digit = qobject_cast<QPushButton*>(sender())->text();
    if (digit == "0" && operand == "0") digit = "";
    if (operand == "0" && digit != "0") operand = "";
    operand += digit;
    ui->display->setText(operand);
}

// 小数点按钮点击
void Widget::on_btnPeriod_clicked()
{
    if (!operand.contains(".") && !operand.isEmpty()) {
        operand += qobject_cast<QPushButton*>(sender())->text();
    }
    ui->display->setText(operand);
}

// 删除按钮
void Widget::on_btnDel_clicked()
{
    operand = operand.left(operand.length() - 1);
    ui->display->setText(operand);
}

// 清除当前输入
void Widget::on_btnClear_clicked()
{
    operand.clear();
    ui->display->setText(operand);
}

// 双目运算符点击


// 单目运算符点击
void Widget::btnUnaryOperatorClicked()
{
    if (!operand.isEmpty()) {
        double result = operand.toDouble();
        operand.clear();
        QString op = qobject_cast<QPushButton*>(sender())->text();

        if (op == "%") {
            result /= 100;
        } else if (op == "1/x") {
            result = 1 / result;
        } else if (op == "x^2") {
            result *= result;
        } else if (op == "√") {
            result = sqrt(result);
        }
        operand = QString::number(result);
        ui->display->setText(operand);
    }
}

// 等号按钮
void Widget::on_btnEqual_clicked()
{
    if (!operand.isEmpty()) {
        operands.push_back(operand);
        operand.clear();
    }
    QString result = calculation();
    operand = result;
    if (!operands.isEmpty()) operands.pop_front();
    ui->display->setText(result);
}

// 键盘事件重写
void Widget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Period) {
        ui->btnPeriod->animateClick();
    }
    foreach (auto btnKey, digitBTNs.keys()) {
        if (event->key() == btnKey) {
            digitBTNs[btnKey]->animateClick();
        }
    }
    QWidget::keyPressEvent(event);  // 调用父类事件处理
}

// 清除所有
void Widget::on_btnClearAll_clicked()
{
    operands.clear();
    opcodes.clear();
    operand.clear();
    ui->display->setText(operand);

}

// 正负号按钮
void Widget::on_btnOpposite_clicked()
{
    if (!operand.isEmpty()) {
        double result = operand.toDouble();
        operand.clear();
        result = -result;
        operand = QString::number(result);
        ui->display->setText(operand);
    }
}
