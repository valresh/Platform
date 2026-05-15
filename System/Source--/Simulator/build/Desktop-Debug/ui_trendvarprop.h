/********************************************************************************
** Form generated from reading UI file 'trendvarprop.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRENDVARPROP_H
#define UI_TRENDVARPROP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <dialcolor.h>

QT_BEGIN_NAMESPACE

class Ui_TrendVarProp
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *Object;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *Param;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *Max;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *Min;
    QHBoxLayout *horizontalLayout_7;
    QCheckBox *UseNom;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QLineEdit *Nom;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    DialColor *color;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *TrendVarProp)
    {
        if (TrendVarProp->objectName().isEmpty())
            TrendVarProp->setObjectName("TrendVarProp");
        TrendVarProp->setWindowModality(Qt::WindowModality::ApplicationModal);
        TrendVarProp->resize(325, 254);
        TrendVarProp->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        TrendVarProp->setModal(false);
        verticalLayout_2 = new QVBoxLayout(TrendVarProp);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(TrendVarProp);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(label);

        Object = new QLineEdit(TrendVarProp);
        Object->setObjectName("Object");

        horizontalLayout->addWidget(Object);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_2 = new QLabel(TrendVarProp);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_2->addWidget(label_2);

        Param = new QLineEdit(TrendVarProp);
        Param->setObjectName("Param");

        horizontalLayout_2->addWidget(Param);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_3 = new QLabel(TrendVarProp);
        label_3->setObjectName("label_3");
        label_3->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_3->addWidget(label_3);

        Max = new QLineEdit(TrendVarProp);
        Max->setObjectName("Max");

        horizontalLayout_3->addWidget(Max);


        horizontalLayout_5->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_4 = new QLabel(TrendVarProp);
        label_4->setObjectName("label_4");
        label_4->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_4->addWidget(label_4);

        Min = new QLineEdit(TrendVarProp);
        Min->setObjectName("Min");

        horizontalLayout_4->addWidget(Min);


        horizontalLayout_5->addLayout(horizontalLayout_4);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        UseNom = new QCheckBox(TrendVarProp);
        UseNom->setObjectName("UseNom");

        horizontalLayout_7->addWidget(UseNom);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        label_5 = new QLabel(TrendVarProp);
        label_5->setObjectName("label_5");
        label_5->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_6->addWidget(label_5);

        Nom = new QLineEdit(TrendVarProp);
        Nom->setObjectName("Nom");
        Nom->setMaximumSize(QSize(80, 16777215));

        horizontalLayout_6->addWidget(Nom);


        horizontalLayout_7->addLayout(horizontalLayout_6);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        label_7 = new QLabel(TrendVarProp);
        label_7->setObjectName("label_7");
        label_7->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_8->addWidget(label_7);

        color = new DialColor(TrendVarProp);
        color->setObjectName("color");
        color->setMinimumSize(QSize(35, 35));
        color->setMaximumSize(QSize(35, 35));

        horizontalLayout_8->addWidget(color);


        horizontalLayout_7->addLayout(horizontalLayout_8);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_7);

        buttonBox = new QDialogButtonBox(TrendVarProp);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Apply|QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout->addWidget(buttonBox);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(TrendVarProp);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, TrendVarProp, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, TrendVarProp, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(TrendVarProp);
    } // setupUi

    void retranslateUi(QDialog *TrendVarProp)
    {
        TrendVarProp->setWindowTitle(QString());
        label->setText(QCoreApplication::translate("TrendVarProp", "    \320\234\320\276\320\264\320\265\320\273\321\214", nullptr));
        label_2->setText(QCoreApplication::translate("TrendVarProp", "\320\237\320\260\321\200\320\262\320\274\320\265\321\202\321\200", nullptr));
        label_3->setText(QCoreApplication::translate("TrendVarProp", " \320\234\320\260\320\272\321\201\320\270\320\274\321\203\320\274", nullptr));
        label_4->setText(QCoreApplication::translate("TrendVarProp", " \320\234\320\270\320\275\320\270\320\274\321\203\320\274", nullptr));
        UseNom->setText(QString());
        label_5->setText(QCoreApplication::translate("TrendVarProp", "   \320\235\320\276\320\274\320\270\320\275\320\260\320\273", nullptr));
        label_7->setText(QCoreApplication::translate("TrendVarProp", "\320\246\320\262\320\265\321\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TrendVarProp: public Ui_TrendVarProp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRENDVARPROP_H
