#ifndef PATIENTVIEW_H
#define PATIENTVIEW_H

#include <QWidget>
#include <QDataWidgetMapper>

namespace Ui {
class patientview;
}

class patientview : public QWidget
{
    Q_OBJECT

public:
    explicit patientview(QWidget *parent = nullptr);
    ~patientview();

private slots:
    void on_btadd_clicked();

    void on_btcheck_clicked();

    void on_btedit_clicked();

    void on_btclear_clicked();

signals:
    void gopatientaddview(int idx);

private:
    Ui::patientview *ui;
     QDataWidgetMapper *patientdatamapper;
};

#endif // PATIENTVIEW_H
