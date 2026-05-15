/********************************************************************************
** Form generated from reading UI file 'showrsu.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWRSU_H
#define UI_SHOWRSU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include "showrsu.h"

QT_BEGIN_NAMESPACE

class Ui_ShowRSU
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QSplitter *splitter_2;
    ObjListTab *ListObj;
    QSplitter *splitter;
    List_W_Tab *List_W;
    List_P_Tab *List_P;
    QHBoxLayout *horizontalLayout;
    QComboBox *Filtr;
    QPushButton *Find;
    QComboBox *File;
    QComboBox *TypeObj;

    void setupUi(QDialog *ShowRSU)
    {
        if (ShowRSU->objectName().isEmpty())
            ShowRSU->setObjectName("ShowRSU");
        ShowRSU->resize(538, 447);
        verticalLayout_2 = new QVBoxLayout(ShowRSU);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        splitter_2 = new QSplitter(ShowRSU);
        splitter_2->setObjectName("splitter_2");
        splitter_2->setOrientation(Qt::Orientation::Horizontal);
        ListObj = new ObjListTab(splitter_2);
        ListObj->setObjectName("ListObj");
        splitter_2->addWidget(ListObj);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Orientation::Vertical);
        List_W = new List_W_Tab(splitter);
        List_W->setObjectName("List_W");
        splitter->addWidget(List_W);
        List_P = new List_P_Tab(splitter);
        List_P->setObjectName("List_P");
        splitter->addWidget(List_P);
        splitter_2->addWidget(splitter);

        verticalLayout->addWidget(splitter_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        Filtr = new QComboBox(ShowRSU);
        Filtr->setObjectName("Filtr");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(60);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Filtr->sizePolicy().hasHeightForWidth());
        Filtr->setSizePolicy(sizePolicy);
        Filtr->setMaximumSize(QSize(16777215, 30));
        Filtr->setEditable(true);

        horizontalLayout->addWidget(Filtr);

        Find = new QPushButton(ShowRSU);
        Find->setObjectName("Find");
        Find->setMaximumSize(QSize(30, 16777215));

        horizontalLayout->addWidget(Find);

        File = new QComboBox(ShowRSU);
        File->setObjectName("File");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(20);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(File->sizePolicy().hasHeightForWidth());
        File->setSizePolicy(sizePolicy1);
        File->setMaximumSize(QSize(16777215, 30));

        horizontalLayout->addWidget(File);

        TypeObj = new QComboBox(ShowRSU);
        TypeObj->setObjectName("TypeObj");
        sizePolicy1.setHeightForWidth(TypeObj->sizePolicy().hasHeightForWidth());
        TypeObj->setSizePolicy(sizePolicy1);
        TypeObj->setMaximumSize(QSize(16777215, 30));

        horizontalLayout->addWidget(TypeObj);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(ShowRSU);

        QMetaObject::connectSlotsByName(ShowRSU);
    } // setupUi

    void retranslateUi(QDialog *ShowRSU)
    {
        ShowRSU->setWindowTitle(QCoreApplication::translate("ShowRSU", "Dialog", nullptr));
        Find->setText(QCoreApplication::translate("ShowRSU", "!", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShowRSU: public Ui_ShowRSU {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWRSU_H
