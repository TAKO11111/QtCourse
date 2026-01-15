#ifndef DEPARTMENTVIEW_H
#define DEPARTMENTVIEW_H

#include <QWidget>

namespace Ui {
class departmentview;
}

class departmentview : public QWidget
{
    Q_OBJECT

public:
    explicit departmentview(QWidget *parent = nullptr);
    ~departmentview();

private:
    Ui::departmentview *ui;
};

#endif // DEPARTMENTVIEW_H
