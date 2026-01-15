#include "widget.h"
#include "ui_widget.h"  // 引用 Widget 的 UI 文件
#include <math.h>
#include <QKeyEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)  // 初始化 UI 为 Widget 对应的 UI
{
    ui->setupUi(this);
    this->setWindowTitle("简易计算器");  // 设置窗口标题
    this->setMinimumSize(300, 400);      // 设置最小尺寸，避免窗口过小

    // 2. 显示框（display）美化
    ui->display->setStyleSheet(R"(
        QLineEdit {
            font-size: 24px;          // 字体大小
            font-weight: bold;        // 加粗
            background-color: #f0f0f0; // 浅灰色背景
            border: 2px solid #ccc;   // 边框
            border-radius: 5px;       // 圆角
            padding: 10px;            // 内边距
            text-align: right;        // 文字右对齐（符合计算器习惯）
        }
    )");
    ui->display->setReadOnly(true);  // 禁止手动输入，只能通过按钮/键盘

    // 3. 数字按钮（0-9、小数点）美化（灰色背景）
    QString numBtnStyle = R"(
        QPushButton {
            font-size: 18px;
            font-weight: bold;
            background-color: #e0e0e0;
            border: 1px solid #ccc;
            border-radius: 8px;
            padding: 10px;
            min-width: 60px;  // 最小宽度，保证按钮大小一致
            min-height: 50px; // 最小高度
        }
        QPushButton:pressed {  // 点击时样式
            background-color: #ccc;
            border: 1px solid #999;
        }
    )";
    // 应用到所有数字按钮和小数点（替换为你的按钮实际名称）
    ui->btnNum0->setStyleSheet(numBtnStyle);
    ui->btnNum1->setStyleSheet(numBtnStyle);
    ui->btnNum2->setStyleSheet(numBtnStyle);
    ui->btnNum3->setStyleSheet(numBtnStyle);
    ui->btnNum4->setStyleSheet(numBtnStyle);
    ui->btnNum5->setStyleSheet(numBtnStyle);
    ui->btnNum6->setStyleSheet(numBtnStyle);
    ui->btnNum7->setStyleSheet(numBtnStyle);
    ui->btnNum8->setStyleSheet(numBtnStyle);
    ui->btnNum9->setStyleSheet(numBtnStyle);
    ui->btnPeriod->setStyleSheet(numBtnStyle);  // 小数点按钮

    // 4. 运算符按钮（+、-、×、/）美化（蓝色背景）
    QString opBtnStyle = R"(
        QPushButton {
            font-size: 18px;
            font-weight: bold;
            background-color: #4a90e2; // 蓝色
            color: white;              // 文字白色
            border: 1px solid #3a80d2;
            border-radius: 8px;
            padding: 10px;
            min-width: 60px;
            min-height: 50px;
        }
        QPushButton:pressed {
            background-color: #3a80d2;
            border: 1px solid #2a70c2;
        }
    )";
    // 应用到双目运算符（替换为你的按钮实际名称）
    ui->btnPlus->setStyleSheet(opBtnStyle);    // +
    ui->btnMinus->setStyleSheet(opBtnStyle);   // -
    ui->btnMultiple->setStyleSheet(opBtnStyle);// ×
    ui->btnDivide->setStyleSheet(opBtnStyle);  // /

    // 5. 功能按钮（清除、等号、正负号等）美化（橙色背景）
    QString funcBtnStyle = R"(
        QPushButton {
            font-size: 18px;
            font-weight: bold;
            background-color: #f5a623; // 橙色
            color: white;
            border: 1px solid #e59613;
            border-radius: 8px;
            padding: 10px;
            min-width: 60px;
            min-height: 50px;
        }
        QPushButton:pressed {
            background-color: #e59613;
            border: 1px solid #d58603;
        }
    )";
    // 应用到功能按钮（替换为你的按钮实际名称）
    ui->btnClear->setStyleSheet(funcBtnStyle);     // 清除
    ui->btnClearAll->setStyleSheet(funcBtnStyle);  // 清除所有
    ui->btnDel->setStyleSheet(funcBtnStyle);       // 删除
    ui->btnEqual->setStyleSheet(funcBtnStyle);     // 等号
    ui->btnOpposite->setStyleSheet(funcBtnStyle);  // 正负号
    ui->btnPercentage->setStyleSheet(funcBtnStyle);// %
    ui->btnInverse->setStyleSheet(funcBtnStyle);   // 1/x
    ui->btnSquare->setStyleSheet(funcBtnStyle);    // x²
    ui->btnSqrt->setStyleSheet(funcBtnStyle);      // √
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
void Widget::binaryOperatorClicked()
{
    QString opcode = qobject_cast<QPushButton*>(sender())->text();
    if (!operand.isEmpty()) {
        operands.push_back(operand);
        operand.clear();
        opcodes.push_back(opcode);
    }
    QString result = calculation();
    ui->display->setText(result);
}

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
