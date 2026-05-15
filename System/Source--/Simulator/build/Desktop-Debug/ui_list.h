/********************************************************************************
** Form generated from reading UI file 'list.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIST_H
#define UI_LIST_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "clistview.h"

QT_BEGIN_NAMESPACE

class Ui_List
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    CListView *Tab;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *OK;
    QPushButton *Cancel;

    void setupUi(QDialog *List)
    {
        if (List->objectName().isEmpty())
            List->setObjectName("List");
        List->resize(240, 320);
        verticalLayout_2 = new QVBoxLayout(List);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        Tab = new CListView(List);
        Tab->setObjectName("Tab");
        Tab->setMinimumSize(QSize(0, 100));
        QFont font;
        font.setFamilies({QString::fromUtf8("Tahoma")});
        font.setPointSize(11);
        font.setBold(true);
        Tab->setFont(font);
        Tab->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
        Tab->setAutoScroll(true);
        Tab->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        Tab->setResizeMode(QListView::ResizeMode::Adjust);

        verticalLayout->addWidget(Tab);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        OK = new QPushButton(List);
        OK->setObjectName("OK");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/OK.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        OK->setIcon(icon);

        horizontalLayout->addWidget(OK);

        Cancel = new QPushButton(List);
        Cancel->setObjectName("Cancel");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Cancel.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Cancel->setIcon(icon1);

        horizontalLayout->addWidget(Cancel);


        horizontalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(List);

        QMetaObject::connectSlotsByName(List);
    } // setupUi

    void retranslateUi(QDialog *List)
    {
        List->setWindowTitle(QCoreApplication::translate("List", "Dialog", nullptr));
        OK->setText(QString());
        Cancel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class List: public Ui_List {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIST_H
