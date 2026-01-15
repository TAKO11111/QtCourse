/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *titleLabel;
    QStackedWidget *stackedWidget;
    QWidget *chatpage;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout;
    QTextEdit *roomtextdit;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEdit;
    QPushButton *saybt;
    QPushButton *logoutbt;
    QWidget *loginpage;
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QFrame *loginFrame;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLineEdit *serveredit;
    QLabel *label_3;
    QLineEdit *userdeit;
    QSpacerItem *verticalSpacer;
    QPushButton *loginbt;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(622, 600);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(120, 0));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        titleLabel = new QLabel(centralwidget);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setGeometry(QRect(6, 6, 81, 16));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(6, 22, 613, 551));
        stackedWidget->setMinimumSize(QSize(120, 0));
        chatpage = new QWidget();
        chatpage->setObjectName("chatpage");
        gridLayout_3 = new QGridLayout(chatpage);
        gridLayout_3->setObjectName("gridLayout_3");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        roomtextdit = new QTextEdit(chatpage);
        roomtextdit->setObjectName("roomtextdit");

        horizontalLayout->addWidget(roomtextdit);

        listWidget = new QListWidget(chatpage);
        listWidget->setObjectName("listWidget");
        sizePolicy.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy);
        listWidget->setMinimumSize(QSize(120, 0));
        listWidget->setMaximumSize(QSize(120, 16777215));

        horizontalLayout->addWidget(listWidget);


        gridLayout_3->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        lineEdit = new QLineEdit(chatpage);
        lineEdit->setObjectName("lineEdit");

        horizontalLayout_2->addWidget(lineEdit);

        saybt = new QPushButton(chatpage);
        saybt->setObjectName("saybt");

        horizontalLayout_2->addWidget(saybt);

        logoutbt = new QPushButton(chatpage);
        logoutbt->setObjectName("logoutbt");

        horizontalLayout_2->addWidget(logoutbt);


        gridLayout_3->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        stackedWidget->addWidget(chatpage);
        loginpage = new QWidget();
        loginpage->setObjectName("loginpage");
        gridLayout_2 = new QGridLayout(loginpage);
        gridLayout_2->setObjectName("gridLayout_2");
        verticalSpacer_3 = new QSpacerItem(20, 211, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer_3, 0, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 211, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 2, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(211, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(211, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 0, 1, 1);

        loginFrame = new QFrame(loginpage);
        loginFrame->setObjectName("loginFrame");
        loginFrame->setFrameShape(QFrame::Shape::StyledPanel);
        loginFrame->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout = new QVBoxLayout(loginFrame);
        verticalLayout->setObjectName("verticalLayout");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label_2 = new QLabel(loginFrame);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        serveredit = new QLineEdit(loginFrame);
        serveredit->setObjectName("serveredit");

        gridLayout->addWidget(serveredit, 0, 1, 1, 1);

        label_3 = new QLabel(loginFrame);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        userdeit = new QLineEdit(loginFrame);
        userdeit->setObjectName("userdeit");

        gridLayout->addWidget(userdeit, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        loginbt = new QPushButton(loginFrame);
        loginbt->setObjectName("loginbt");

        verticalLayout->addWidget(loginbt);


        gridLayout_2->addWidget(loginFrame, 1, 1, 1, 1);

        stackedWidget->addWidget(loginpage);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\350\201\212\345\244\251\345\256\244\345\256\242\346\210\267\347\253\257", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "\346\254\247\351\230\263\346\233\276\351\224\246\345\237\271\347\232\204\350\201\212\345\244\251\345\256\244", nullptr));
        saybt->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        logoutbt->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\346\234\215\345\212\241\345\231\250\345\234\260\345\235\200\357\274\232", nullptr));
        serveredit->setText(QCoreApplication::translate("MainWindow", "127.0.0.1", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\346\230\265\347\247\260\357\274\232", nullptr));
        loginbt->setText(QCoreApplication::translate("MainWindow", "\347\231\273\351\231\206\350\201\212\345\244\251\345\256\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
