#ifndef WELCOMEVIEW_H
#define WELCOMEVIEW_H

#include <QWidget>

namespace Ui {
class welcomeview;
}

class welcomeview : public QWidget
{
    Q_OBJECT

public:
    explicit welcomeview(QWidget *parent = nullptr);
    ~welcomeview();

private slots:
    void on_btpart_clicked();

    void on_btdo_clicked();

    void on_btpa_clicked();

signals:
    void godepartmentview();
    void godoctorview();
    void gopatientview();

private:
    Ui::welcomeview *ui;
};

#endif // WELCOMEVIEW_H
