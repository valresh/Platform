/********************************************************************************
** Form generated from reading UI file 'minitrend.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MINITREND_H
#define UI_MINITREND_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Trends/minidraw.h"

QT_BEGIN_NAMESPACE

class Ui_minitrend
{
public:
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QComboBox *VarList;
    QSlider *ScaleT;
    QLineEdit *step;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *pow;
    QSlider *ScaleV;
    QVBoxLayout *verticalLayout;
    minidraw *Wnd;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *Back;
    QPushButton *Forvard;
    QPushButton *Pause;
    QPushButton *Continue;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *minitrend)
    {
        if (minitrend->objectName().isEmpty())
            minitrend->setObjectName("minitrend");
        minitrend->resize(450, 198);
        verticalLayout_4 = new QVBoxLayout(minitrend);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        VarList = new QComboBox(minitrend);
        VarList->setObjectName("VarList");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(60);
        sizePolicy.setVerticalStretch(10);
        sizePolicy.setHeightForWidth(VarList->sizePolicy().hasHeightForWidth());
        VarList->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(VarList);

        ScaleT = new QSlider(minitrend);
        ScaleT->setObjectName("ScaleT");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(40);
        sizePolicy1.setVerticalStretch(10);
        sizePolicy1.setHeightForWidth(ScaleT->sizePolicy().hasHeightForWidth());
        ScaleT->setSizePolicy(sizePolicy1);
        ScaleT->setOrientation(Qt::Orientation::Horizontal);

        horizontalLayout->addWidget(ScaleT);

        step = new QLineEdit(minitrend);
        step->setObjectName("step");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(10);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(step->sizePolicy().hasHeightForWidth());
        step->setSizePolicy(sizePolicy2);
        step->setReadOnly(true);

        horizontalLayout->addWidget(step);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        pow = new QLineEdit(minitrend);
        pow->setObjectName("pow");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(10);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(pow->sizePolicy().hasHeightForWidth());
        pow->setSizePolicy(sizePolicy3);
        pow->setMinimumSize(QSize(30, 0));
        pow->setMaximumSize(QSize(30, 16777215));
        pow->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        pow->setReadOnly(true);

        verticalLayout_2->addWidget(pow);

        ScaleV = new QSlider(minitrend);
        ScaleV->setObjectName("ScaleV");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Expanding);
        sizePolicy4.setHorizontalStretch(10);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(ScaleV->sizePolicy().hasHeightForWidth());
        ScaleV->setSizePolicy(sizePolicy4);
        ScaleV->setMinimumSize(QSize(30, 0));
        ScaleV->setOrientation(Qt::Orientation::Vertical);

        verticalLayout_2->addWidget(ScaleV);


        horizontalLayout_3->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        Wnd = new minidraw(minitrend);
        Wnd->setObjectName("Wnd");
        QSizePolicy sizePolicy5(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy5.setHorizontalStretch(90);
        sizePolicy5.setVerticalStretch(90);
        sizePolicy5.setHeightForWidth(Wnd->sizePolicy().hasHeightForWidth());
        Wnd->setSizePolicy(sizePolicy5);

        verticalLayout->addWidget(Wnd);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(12);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        Back = new QPushButton(minitrend);
        Back->setObjectName("Back");
        QSizePolicy sizePolicy6(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(Back->sizePolicy().hasHeightForWidth());
        Back->setSizePolicy(sizePolicy6);
        Back->setMaximumSize(QSize(25, 16777215));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons8-\320\277\320\265\321\200\320\265\320\274\320\276\321\202\320\272\320\260-\320\275\320\260\320\267\320\260\320\264-30.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Back->setIcon(icon);

        horizontalLayout_2->addWidget(Back);

        Forvard = new QPushButton(minitrend);
        Forvard->setObjectName("Forvard");
        sizePolicy6.setHeightForWidth(Forvard->sizePolicy().hasHeightForWidth());
        Forvard->setSizePolicy(sizePolicy6);
        Forvard->setMaximumSize(QSize(25, 16777215));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons8-fast-forward-30.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Forvard->setIcon(icon1);

        horizontalLayout_2->addWidget(Forvard);

        Pause = new QPushButton(minitrend);
        Pause->setObjectName("Pause");
        sizePolicy6.setHeightForWidth(Pause->sizePolicy().hasHeightForWidth());
        Pause->setSizePolicy(sizePolicy6);
        Pause->setMaximumSize(QSize(25, 16777215));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/pause_24.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Pause->setIcon(icon2);

        horizontalLayout_2->addWidget(Pause);

        Continue = new QPushButton(minitrend);
        Continue->setObjectName("Continue");
        sizePolicy6.setHeightForWidth(Continue->sizePolicy().hasHeightForWidth());
        Continue->setSizePolicy(sizePolicy6);
        Continue->setMaximumSize(QSize(25, 16777215));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons8-\320\277\321\200\320\276\320\264\320\276\320\273\320\266\320\270\321\202\321\214-\320\262\320\276\321\201\320\277\321\200\320\276\320\270\320\267\320\262\320\265\320\264\320\265\320\275\320\270\320\265-50.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Continue->setIcon(icon3);

        horizontalLayout_2->addWidget(Continue);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout);


        verticalLayout_3->addLayout(horizontalLayout_3);


        verticalLayout_4->addLayout(verticalLayout_3);


        retranslateUi(minitrend);

        QMetaObject::connectSlotsByName(minitrend);
    } // setupUi

    void retranslateUi(QWidget *minitrend)
    {
        minitrend->setWindowTitle(QCoreApplication::translate("minitrend", "Form", nullptr));
        Back->setText(QString());
        Forvard->setText(QString());
        Pause->setText(QString());
        Continue->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class minitrend: public Ui_minitrend {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MINITREND_H
