/********************************************************************************
** Form generated from reading UI file 'showparams.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWPARAMS_H
#define UI_SHOWPARAMS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "paramslist.h"
#include "paramstree.h"

QT_BEGIN_NAMESPACE

class Ui_ShowParams
{
public:
    QAction *Close;
    QAction *Param_Ref;
    QAction *Copy_Obj_Name;
    QAction *Add_favorit;
    QAction *Remove_favorit;
    QAction *Save_Obj_Param;
    QAction *Rest_Obj_Param;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout;
    QPushButton *P_1;
    QLabel *label;
    QPushButton *M_1;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *P_01;
    QLabel *label_2;
    QPushButton *M_01;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *P_001;
    QPushButton *Sign;
    QPushButton *M_001;
    QPushButton *Step1;
    QCheckBox *bp;
    QSplitter *splitter;
    ParamsTree *ParamTree;
    ParamsList *ParamList;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;

    void setupUi(QMainWindow *ShowParams)
    {
        if (ShowParams->objectName().isEmpty())
            ShowParams->setObjectName("ShowParams");
        ShowParams->resize(569, 382);
        Close = new QAction(ShowParams);
        Close->setObjectName("Close");
        Param_Ref = new QAction(ShowParams);
        Param_Ref->setObjectName("Param_Ref");
        Copy_Obj_Name = new QAction(ShowParams);
        Copy_Obj_Name->setObjectName("Copy_Obj_Name");
        Add_favorit = new QAction(ShowParams);
        Add_favorit->setObjectName("Add_favorit");
        Remove_favorit = new QAction(ShowParams);
        Remove_favorit->setObjectName("Remove_favorit");
        Save_Obj_Param = new QAction(ShowParams);
        Save_Obj_Param->setObjectName("Save_Obj_Param");
        Rest_Obj_Param = new QAction(ShowParams);
        Rest_Obj_Param->setObjectName("Rest_Obj_Param");
        centralwidget = new QWidget(ShowParams);
        centralwidget->setObjectName("centralwidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        P_1 = new QPushButton(centralwidget);
        P_1->setObjectName("P_1");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(P_1->sizePolicy().hasHeightForWidth());
        P_1->setSizePolicy(sizePolicy1);
        P_1->setMinimumSize(QSize(25, 25));
        P_1->setMaximumSize(QSize(25, 25));
        QFont font;
        font.setFamilies({QString::fromUtf8("Tahoma")});
        font.setPointSize(12);
        font.setBold(true);
        P_1->setFont(font);

        horizontalLayout->addWidget(P_1);

        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setMaximumSize(QSize(20, 16777215));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Tahoma")});
        font1.setPointSize(10);
        font1.setBold(true);
        label->setFont(font1);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(label);

        M_1 = new QPushButton(centralwidget);
        M_1->setObjectName("M_1");
        sizePolicy1.setHeightForWidth(M_1->sizePolicy().hasHeightForWidth());
        M_1->setSizePolicy(sizePolicy1);
        M_1->setMinimumSize(QSize(25, 25));
        M_1->setMaximumSize(QSize(25, 25));
        M_1->setFont(font);

        horizontalLayout->addWidget(M_1);


        horizontalLayout_4->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        P_01 = new QPushButton(centralwidget);
        P_01->setObjectName("P_01");
        sizePolicy1.setHeightForWidth(P_01->sizePolicy().hasHeightForWidth());
        P_01->setSizePolicy(sizePolicy1);
        P_01->setMinimumSize(QSize(25, 25));
        P_01->setMaximumSize(QSize(25, 25));
        P_01->setFont(font);

        horizontalLayout_2->addWidget(P_01);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);
        label_2->setMinimumSize(QSize(35, 25));
        label_2->setMaximumSize(QSize(35, 25));
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_2->addWidget(label_2);

        M_01 = new QPushButton(centralwidget);
        M_01->setObjectName("M_01");
        sizePolicy1.setHeightForWidth(M_01->sizePolicy().hasHeightForWidth());
        M_01->setSizePolicy(sizePolicy1);
        M_01->setMinimumSize(QSize(25, 25));
        M_01->setMaximumSize(QSize(25, 25));
        M_01->setFont(font);

        horizontalLayout_2->addWidget(M_01);


        horizontalLayout_4->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        P_001 = new QPushButton(centralwidget);
        P_001->setObjectName("P_001");
        sizePolicy1.setHeightForWidth(P_001->sizePolicy().hasHeightForWidth());
        P_001->setSizePolicy(sizePolicy1);
        P_001->setMinimumSize(QSize(25, 25));
        P_001->setMaximumSize(QSize(25, 25));
        P_001->setFont(font);

        horizontalLayout_3->addWidget(P_001);

        Sign = new QPushButton(centralwidget);
        Sign->setObjectName("Sign");
        sizePolicy1.setHeightForWidth(Sign->sizePolicy().hasHeightForWidth());
        Sign->setSizePolicy(sizePolicy1);
        Sign->setMinimumSize(QSize(40, 25));
        Sign->setMaximumSize(QSize(40, 25));
        Sign->setFont(font1);

        horizontalLayout_3->addWidget(Sign);

        M_001 = new QPushButton(centralwidget);
        M_001->setObjectName("M_001");
        sizePolicy1.setHeightForWidth(M_001->sizePolicy().hasHeightForWidth());
        M_001->setSizePolicy(sizePolicy1);
        M_001->setMinimumSize(QSize(25, 25));
        M_001->setMaximumSize(QSize(25, 25));
        M_001->setFont(font);

        horizontalLayout_3->addWidget(M_001);


        horizontalLayout_4->addLayout(horizontalLayout_3);

        Step1 = new QPushButton(centralwidget);
        Step1->setObjectName("Step1");
        sizePolicy1.setHeightForWidth(Step1->sizePolicy().hasHeightForWidth());
        Step1->setSizePolicy(sizePolicy1);
        Step1->setMinimumSize(QSize(15, 15));
        Step1->setMaximumSize(QSize(15, 15));
        Step1->setFont(font);

        horizontalLayout_4->addWidget(Step1);

        bp = new QCheckBox(centralwidget);
        bp->setObjectName("bp");

        horizontalLayout_4->addWidget(bp);


        verticalLayout->addLayout(horizontalLayout_4);

        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(1);
        sizePolicy3.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy3);
        splitter->setOrientation(Qt::Orientation::Horizontal);
        ParamTree = new ParamsTree(splitter);
        ParamTree->setObjectName("ParamTree");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy4.setHorizontalStretch(4);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(ParamTree->sizePolicy().hasHeightForWidth());
        ParamTree->setSizePolicy(sizePolicy4);
        ParamTree->setMinimumSize(QSize(0, 0));
        ParamTree->setBaseSize(QSize(300, 800));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Tahoma")});
        font2.setPointSize(11);
        font2.setBold(true);
        ParamTree->setFont(font2);
        splitter->addWidget(ParamTree);
        ParamList = new ParamsList(splitter);
        ParamList->setObjectName("ParamList");
        QSizePolicy sizePolicy5(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy5.setHorizontalStretch(6);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(ParamList->sizePolicy().hasHeightForWidth());
        ParamList->setSizePolicy(sizePolicy5);
        ParamList->setMinimumSize(QSize(0, 0));
        ParamList->setBaseSize(QSize(400, 800));
        ParamList->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked);
        ParamList->setGridStyle(Qt::PenStyle::DotLine);
        splitter->addWidget(ParamList);
        ParamList->horizontalHeader()->setStretchLastSection(true);
        ParamList->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(splitter);


        verticalLayout_2->addLayout(verticalLayout);

        ShowParams->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ShowParams);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 569, 22));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName("menu_2");
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName("menu_3");
        menu_4 = new QMenu(menubar);
        menu_4->setObjectName("menu_4");
        ShowParams->setMenuBar(menubar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menubar->addAction(menu_4->menuAction());
        menu->addAction(Close);
        menu_2->addAction(Param_Ref);
        menu_2->addAction(Save_Obj_Param);
        menu_2->addAction(Rest_Obj_Param);
        menu_3->addAction(Copy_Obj_Name);
        menu_4->addAction(Add_favorit);
        menu_4->addAction(Remove_favorit);

        retranslateUi(ShowParams);

        QMetaObject::connectSlotsByName(ShowParams);
    } // setupUi

    void retranslateUi(QMainWindow *ShowParams)
    {
        ShowParams->setWindowTitle(QCoreApplication::translate("ShowParams", "MainWindow", nullptr));
        Close->setText(QCoreApplication::translate("ShowParams", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", nullptr));
        Param_Ref->setText(QCoreApplication::translate("ShowParams", "\320\241\321\201\321\213\320\273\320\272\320\260 \320\275\320\260 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200", nullptr));
        Copy_Obj_Name->setText(QCoreApplication::translate("ShowParams", "\320\232\320\276\320\277\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\270\320\274\321\217", nullptr));
        Add_favorit->setText(QCoreApplication::translate("ShowParams", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", nullptr));
        Remove_favorit->setText(QCoreApplication::translate("ShowParams", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        Save_Obj_Param->setText(QCoreApplication::translate("ShowParams", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        Rest_Obj_Param->setText(QCoreApplication::translate("ShowParams", "\320\237\321\200\320\276\321\207\320\270\321\202\320\260\321\202\321\214", nullptr));
        P_1->setText(QCoreApplication::translate("ShowParams", "+", nullptr));
        label->setText(QCoreApplication::translate("ShowParams", "1%", nullptr));
        M_1->setText(QCoreApplication::translate("ShowParams", "-", nullptr));
        P_01->setText(QCoreApplication::translate("ShowParams", "+", nullptr));
        label_2->setText(QCoreApplication::translate("ShowParams", "0.1%", nullptr));
        M_01->setText(QCoreApplication::translate("ShowParams", "-", nullptr));
        P_001->setText(QCoreApplication::translate("ShowParams", "+", nullptr));
        Sign->setText(QCoreApplication::translate("ShowParams", "0.01", nullptr));
        M_001->setText(QCoreApplication::translate("ShowParams", "-", nullptr));
        Step1->setText(QCoreApplication::translate("ShowParams", "+", nullptr));
        bp->setText(QString());
        menu->setTitle(QCoreApplication::translate("ShowParams", "\320\244\320\260\320\271\320\273", nullptr));
        menu_2->setTitle(QCoreApplication::translate("ShowParams", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213", nullptr));
        menu_3->setTitle(QCoreApplication::translate("ShowParams", "\320\236\320\261\321\212\320\265\320\272\321\202", nullptr));
        menu_4->setTitle(QCoreApplication::translate("ShowParams", "\320\230\320\267\320\261\321\200\320\260\320\275\320\275\320\276\320\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShowParams: public Ui_ShowParams {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWPARAMS_H
