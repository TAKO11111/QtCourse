#ifndef PATIENTADDVIEW_H
#define PATIENTADDVIEW_H

#include <QWidget>

namespace Ui {
class patientaddview;
}

class patientaddview : public QWidget
{
    Q_OBJECT

public:
    explicit patientaddview(QWidget *parent = nullptr,int index=0);
    ~patientaddview();

private:
    Ui::patientaddview *ui;
};

#endif // PATIENTADDVIEW_H
