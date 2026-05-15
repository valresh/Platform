/********************************************************************************
** Form generated from reading UI file 'trendsvar.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRENDSVAR_H
#define UI_TRENDSVAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <qtableviewmouse.h>

QT_BEGIN_NAMESPACE

class Ui_TrendsVar
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTableViewMouse *Wnd;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *TrendsVar)
    {
        if (TrendsVar->objectName().isEmpty())
            TrendsVar->setObjectName("TrendsVar");
        TrendsVar->resize(240, 320);
        centralwidget = new QWidget(TrendsVar);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        Wnd = new QTableViewMouse(centralwidget);
        Wnd->setObjectName("Wnd");

        verticalLayout->addWidget(Wnd);

        TrendsVar->setCentralWidget(centralwidget);
        menubar = new QMenuBar(TrendsVar);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 240, 22));
        TrendsVar->setMenuBar(menubar);
        statusbar = new QStatusBar(TrendsVar);
        statusbar->setObjectName("statusbar");
        TrendsVar->setStatusBar(statusbar);

        retranslateUi(TrendsVar);

        QMetaObject::connectSlotsByName(TrendsVar);
    } // setupUi

    void retranslateUi(QMainWindow *TrendsVar)
    {
        TrendsVar->setWindowTitle(QCoreApplication::translate("TrendsVar", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TrendsVar: public Ui_TrendsVar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRENDSVAR_H
