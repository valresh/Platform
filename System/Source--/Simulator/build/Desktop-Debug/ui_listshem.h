/********************************************************************************
** Form generated from reading UI file 'listshem.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTSHEM_H
#define UI_LISTSHEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QVBoxLayout>
#include "listemf.h"

QT_BEGIN_NAMESPACE

class Ui_ListShem
{
public:
    QVBoxLayout *verticalLayout;
    ListEMF *List;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ListShem)
    {
        if (ListShem->objectName().isEmpty())
            ListShem->setObjectName("ListShem");
        ListShem->resize(240, 451);
        verticalLayout = new QVBoxLayout(ListShem);
        verticalLayout->setObjectName("verticalLayout");
        List = new ListEMF(ListShem);
        List->setObjectName("List");
        List->setMouseTracking(true);
        List->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);
        List->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        List->setSelectionRectVisible(false);

        verticalLayout->addWidget(List);

        buttonBox = new QDialogButtonBox(ListShem);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ListShem);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, ListShem, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, ListShem, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(ListShem);
    } // setupUi

    void retranslateUi(QDialog *ListShem)
    {
        ListShem->setWindowTitle(QCoreApplication::translate("ListShem", "\320\222\321\213\320\261\320\276\321\200 \321\201\321\205\320\265\320\274\321\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ListShem: public Ui_ListShem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTSHEM_H
