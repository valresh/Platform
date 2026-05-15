/********************************************************************************
** Form generated from reading UI file 'findobj.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDOBJ_H
#define UI_FINDOBJ_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FindObj
{
public:
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QTableView *Tab;
    QHBoxLayout *horizontalLayout;
    QComboBox *Filtr;
    QVBoxLayout *verticalLayout;
    QCheckBox *OnSheme;
    QLabel *label;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *Modeles;
    QLabel *label_2;
    QPushButton *Find;
    QPushButton *OK;

    void setupUi(QDialog *FindObj)
    {
        if (FindObj->objectName().isEmpty())
            FindObj->setObjectName("FindObj");
        FindObj->resize(365, 464);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/book.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        FindObj->setWindowIcon(icon);
        verticalLayout_4 = new QVBoxLayout(FindObj);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        Tab = new QTableView(FindObj);
        Tab->setObjectName("Tab");

        verticalLayout_3->addWidget(Tab);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        Filtr = new QComboBox(FindObj);
        Filtr->setObjectName("Filtr");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(150);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Filtr->sizePolicy().hasHeightForWidth());
        Filtr->setSizePolicy(sizePolicy);
        Filtr->setEditable(true);

        horizontalLayout->addWidget(Filtr);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        OnSheme = new QCheckBox(FindObj);
        OnSheme->setObjectName("OnSheme");
        OnSheme->setChecked(true);

        verticalLayout->addWidget(OnSheme);

        label = new QLabel(FindObj);
        label->setObjectName("label");

        verticalLayout->addWidget(label);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        Modeles = new QCheckBox(FindObj);
        Modeles->setObjectName("Modeles");

        verticalLayout_2->addWidget(Modeles);

        label_2 = new QLabel(FindObj);
        label_2->setObjectName("label_2");

        verticalLayout_2->addWidget(label_2);


        horizontalLayout->addLayout(verticalLayout_2);

        Find = new QPushButton(FindObj);
        Find->setObjectName("Find");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Find->sizePolicy().hasHeightForWidth());
        Find->setSizePolicy(sizePolicy1);
        Find->setMinimumSize(QSize(25, 25));
        Find->setMaximumSize(QSize(25, 25));

        horizontalLayout->addWidget(Find);

        OK = new QPushButton(FindObj);
        OK->setObjectName("OK");
        sizePolicy1.setHeightForWidth(OK->sizePolicy().hasHeightForWidth());
        OK->setSizePolicy(sizePolicy1);
        OK->setMinimumSize(QSize(25, 24));
        OK->setMaximumSize(QSize(25, 25));

        horizontalLayout->addWidget(OK);


        verticalLayout_3->addLayout(horizontalLayout);


        verticalLayout_4->addLayout(verticalLayout_3);


        retranslateUi(FindObj);

        QMetaObject::connectSlotsByName(FindObj);
    } // setupUi

    void retranslateUi(QDialog *FindObj)
    {
        FindObj->setWindowTitle(QCoreApplication::translate("FindObj", "Dialog", nullptr));
        OnSheme->setText(QString());
        label->setText(QCoreApplication::translate("FindObj", "\321\201\321\205\320\265\320\274\321\213", nullptr));
        Modeles->setText(QString());
        label_2->setText(QCoreApplication::translate("FindObj", "\320\274\320\276\320\264\320\265\320\273\320\270", nullptr));
        Find->setText(QCoreApplication::translate("FindObj", "!", nullptr));
        OK->setText(QCoreApplication::translate("FindObj", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FindObj: public Ui_FindObj {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDOBJ_H
