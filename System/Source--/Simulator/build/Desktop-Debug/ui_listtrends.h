/********************************************************************************
** Form generated from reading UI file 'listtrends.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTTRENDS_H
#define UI_LISTTRENDS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QVBoxLayout>
#include "listemf.h"

QT_BEGIN_NAMESPACE

class Ui_ListTrends
{
public:
    QVBoxLayout *verticalLayout;
    ListEMF *List;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ListTrends)
    {
        if (ListTrends->objectName().isEmpty())
            ListTrends->setObjectName("ListTrends");
        ListTrends->resize(184, 191);
        verticalLayout = new QVBoxLayout(ListTrends);
        verticalLayout->setObjectName("verticalLayout");
        List = new ListEMF(ListTrends);
        List->setObjectName("List");
        List->setMouseTracking(true);
        List->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);
        List->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        List->setSelectionRectVisible(false);

        verticalLayout->addWidget(List);

        buttonBox = new QDialogButtonBox(ListTrends);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ListTrends);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, ListTrends, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, ListTrends, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(ListTrends);
    } // setupUi

    void retranslateUi(QDialog *ListTrends)
    {
        ListTrends->setWindowTitle(QCoreApplication::translate("ListTrends", "\320\222\321\213\320\261\320\276\321\200 \321\201\321\205\320\265\320\274\321\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ListTrends: public Ui_ListTrends {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTTRENDS_H
