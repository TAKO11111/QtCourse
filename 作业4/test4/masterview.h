#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QWidget>
#include "loginview.h"
#include "doctorview.h"
#include "welcomeview.h"
#include "departmentview.h"
#include "patientaddview.h"
#include "patientview.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MasterView;
}
QT_END_NAMESPACE

class MasterView : public QWidget
{
    Q_OBJECT

public:
    MasterView(QWidget *parent = nullptr);
    ~MasterView();

public slots:
    void gologinview();
    void godepartmentview();
    void godoctorview();
    void gopatientaddview(int rowno);
    void gopatientview();
    void gowelcomeview();
    void gopreviousview();

private slots:
    void on_btBack_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_btLogout_clicked();

private:
    void pushview(QWidget *widget);

    Ui::MasterView *ui;

    LoginView *loginview;
    welcomeview *welcomeview;
    doctorview *doctorview;
    departmentview *departmentview;
    patientaddview *patientaddview;
    patientview *patientview;
};
#endif // MASTERVIEW_H
