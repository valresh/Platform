/********************************************************************************
** Form generated from reading UI file 'qtrends.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTRENDS_H
#define UI_QTRENDS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Trends/showtrends.h"

QT_BEGIN_NAMESPACE

class Ui_QTrends
{
public:
    QAction *action;
    QAction *action_2;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *ShowVars;
    QLineEdit *SelVar;
    QLineEdit *SelValue;
    QSpacerItem *horizontalSpacer;
    QPushButton *Back;
    QPushButton *Forvard;
    QPushButton *Pause;
    QPushButton *Continue;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *Up;
    QPushButton *Down;
    QPushButton *Align;
    QSpacerItem *horizontalSpacer_3;
    QSlider *ScaleT;
    QLineEdit *step;
    QPushButton *TimeStep;
    QSpacerItem *horizontalSpacer_4;
    ShowTrends *Wnd;
    QMenuBar *menubar;
    QMenu *menu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *QTrends)
    {
        if (QTrends->objectName().isEmpty())
            QTrends->setObjectName("QTrends");
        QTrends->resize(958, 691);
        action = new QAction(QTrends);
        action->setObjectName("action");
        action_2 = new QAction(QTrends);
        action_2->setObjectName("action_2");
        centralwidget = new QWidget(QTrends);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        ShowVars = new QPushButton(centralwidget);
        ShowVars->setObjectName("ShowVars");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/List.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        ShowVars->setIcon(icon);

        horizontalLayout->addWidget(ShowVars);

        SelVar = new QLineEdit(centralwidget);
        SelVar->setObjectName("SelVar");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(200);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SelVar->sizePolicy().hasHeightForWidth());
        SelVar->setSizePolicy(sizePolicy);
        SelVar->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(SelVar);

        SelValue = new QLineEdit(centralwidget);
        SelValue->setObjectName("SelValue");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(90);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(SelValue->sizePolicy().hasHeightForWidth());
        SelValue->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(SelValue);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        Back = new QPushButton(centralwidget);
        Back->setObjectName("Back");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(Back->sizePolicy().hasHeightForWidth());
        Back->setSizePolicy(sizePolicy2);
        Back->setMaximumSize(QSize(25, 16777215));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons8-\320\277\320\265\321\200\320\265\320\274\320\276\321\202\320\272\320\260-\320\275\320\260\320\267\320\260\320\264-30.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Back->setIcon(icon1);

        horizontalLayout->addWidget(Back);

        Forvard = new QPushButton(centralwidget);
        Forvard->setObjectName("Forvard");
        sizePolicy2.setHeightForWidth(Forvard->sizePolicy().hasHeightForWidth());
        Forvard->setSizePolicy(sizePolicy2);
        Forvard->setMaximumSize(QSize(25, 16777215));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons8-fast-forward-30.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Forvard->setIcon(icon2);

        horizontalLayout->addWidget(Forvard);

        Pause = new QPushButton(centralwidget);
        Pause->setObjectName("Pause");
        sizePolicy2.setHeightForWidth(Pause->sizePolicy().hasHeightForWidth());
        Pause->setSizePolicy(sizePolicy2);
        Pause->setMaximumSize(QSize(25, 16777215));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/pause_24.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Pause->setIcon(icon3);

        horizontalLayout->addWidget(Pause);

        Continue = new QPushButton(centralwidget);
        Continue->setObjectName("Continue");
        sizePolicy2.setHeightForWidth(Continue->sizePolicy().hasHeightForWidth());
        Continue->setSizePolicy(sizePolicy2);
        Continue->setMaximumSize(QSize(25, 16777215));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons8-\320\277\321\200\320\276\320\264\320\276\320\273\320\266\320\270\321\202\321\214-\320\262\320\276\321\201\320\277\321\200\320\276\320\270\320\267\320\262\320\265\320\264\320\265\320\275\320\270\320\265-50.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Continue->setIcon(icon4);

        horizontalLayout->addWidget(Continue);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        Up = new QPushButton(centralwidget);
        Up->setObjectName("Up");
        sizePolicy2.setHeightForWidth(Up->sizePolicy().hasHeightForWidth());
        Up->setSizePolicy(sizePolicy2);
        Up->setMaximumSize(QSize(25, 16777215));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/Up.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Up->setIcon(icon5);

        horizontalLayout->addWidget(Up);

        Down = new QPushButton(centralwidget);
        Down->setObjectName("Down");
        sizePolicy2.setHeightForWidth(Down->sizePolicy().hasHeightForWidth());
        Down->setSizePolicy(sizePolicy2);
        Down->setMaximumSize(QSize(25, 16777215));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/Down.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Down->setIcon(icon6);

        horizontalLayout->addWidget(Down);

        Align = new QPushButton(centralwidget);
        Align->setObjectName("Align");
        sizePolicy2.setHeightForWidth(Align->sizePolicy().hasHeightForWidth());
        Align->setSizePolicy(sizePolicy2);
        Align->setMaximumSize(QSize(25, 16777215));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/Align.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Align->setIcon(icon7);

        horizontalLayout->addWidget(Align);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        ScaleT = new QSlider(centralwidget);
        ScaleT->setObjectName("ScaleT");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(100);
        sizePolicy3.setVerticalStretch(10);
        sizePolicy3.setHeightForWidth(ScaleT->sizePolicy().hasHeightForWidth());
        ScaleT->setSizePolicy(sizePolicy3);
        ScaleT->setMaximumSize(QSize(200, 16777215));
        ScaleT->setOrientation(Qt::Orientation::Horizontal);

        horizontalLayout->addWidget(ScaleT);

        step = new QLineEdit(centralwidget);
        step->setObjectName("step");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy4.setHorizontalStretch(10);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(step->sizePolicy().hasHeightForWidth());
        step->setSizePolicy(sizePolicy4);
        step->setMinimumSize(QSize(80, 0));
        step->setMaximumSize(QSize(60, 16777215));
        step->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        step->setReadOnly(true);

        horizontalLayout->addWidget(step);

        TimeStep = new QPushButton(centralwidget);
        TimeStep->setObjectName("TimeStep");
        TimeStep->setMaximumSize(QSize(90, 16777215));
        TimeStep->setBaseSize(QSize(100, 0));

        horizontalLayout->addWidget(TimeStep);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        horizontalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addLayout(horizontalLayout_2);

        Wnd = new ShowTrends(centralwidget);
        Wnd->setObjectName("Wnd");
        QSizePolicy sizePolicy5(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(90);
        sizePolicy5.setHeightForWidth(Wnd->sizePolicy().hasHeightForWidth());
        Wnd->setSizePolicy(sizePolicy5);

        verticalLayout->addWidget(Wnd);


        verticalLayout_2->addLayout(verticalLayout);

        QTrends->setCentralWidget(centralwidget);
        menubar = new QMenuBar(QTrends);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 958, 22));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        QTrends->setMenuBar(menubar);
        statusbar = new QStatusBar(QTrends);
        statusbar->setObjectName("statusbar");
        QTrends->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menu->addAction(action);
        menu->addAction(action_2);

        retranslateUi(QTrends);

        QMetaObject::connectSlotsByName(QTrends);
    } // setupUi

    void retranslateUi(QMainWindow *QTrends)
    {
        QTrends->setWindowTitle(QCoreApplication::translate("QTrends", "MainWindow", nullptr));
        action->setText(QCoreApplication::translate("QTrends", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", nullptr));
        action_2->setText(QCoreApplication::translate("QTrends", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", nullptr));
        ShowVars->setText(QString());
        Back->setText(QString());
        Forvard->setText(QString());
        Pause->setText(QString());
        Continue->setText(QString());
        Up->setText(QString());
        Down->setText(QString());
        Align->setText(QString());
        TimeStep->setText(QCoreApplication::translate("QTrends", "\320\222\321\200\320\265\320\274\321\217", nullptr));
        menu->setTitle(QCoreApplication::translate("QTrends", "\320\244\320\260\320\271\320\273", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QTrends: public Ui_QTrends {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTRENDS_H
