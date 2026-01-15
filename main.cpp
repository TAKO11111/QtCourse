#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;  // 实例化核心类 Widget
    w.show();  // 显示窗口

    return a.exec();
}
