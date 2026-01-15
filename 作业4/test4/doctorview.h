#ifndef DOCTORVIEW_H
#define DOCTORVIEW_H

#include <QWidget>

namespace Ui {
class doctorview;
}

class doctorview : public QWidget
{
    Q_OBJECT

public:
    explicit doctorview(QWidget *parent = nullptr);
    ~doctorview();

private:
    Ui::doctorview *ui;
};

#endif // DOCTORVIEW_H
