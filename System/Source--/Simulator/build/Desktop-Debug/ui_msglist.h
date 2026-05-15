/********************************************************************************
** Form generated from reading UI file 'msglist.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MSGLIST_H
#define UI_MSGLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QListView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MsgList
{
public:
    QVBoxLayout *verticalLayout;
    QListView *List;

    void setupUi(QDialog *MsgList)
    {
        if (MsgList->objectName().isEmpty())
            MsgList->setObjectName("MsgList");
        MsgList->resize(525, 484);
        verticalLayout = new QVBoxLayout(MsgList);
        verticalLayout->setObjectName("verticalLayout");
        List = new QListView(MsgList);
        List->setObjectName("List");
        QFont font;
        font.setFamilies({QString::fromUtf8("Tahoma")});
        font.setPointSize(12);
        font.setBold(true);
        List->setFont(font);

        verticalLayout->addWidget(List);


        retranslateUi(MsgList);

        QMetaObject::connectSlotsByName(MsgList);
    } // setupUi

    void retranslateUi(QDialog *MsgList)
    {
        MsgList->setWindowTitle(QCoreApplication::translate("MsgList", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MsgList: public Ui_MsgList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MSGLIST_H
