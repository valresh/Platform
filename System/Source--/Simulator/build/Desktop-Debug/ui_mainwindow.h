/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *exit;
    QAction *action;
    QWidget *centralwidget;
    QLineEdit *Msg;
    QLineEdit *Project;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *Shema;
    QPushButton *Trends;
    QPushButton *Messages;
    QPushButton *RSU;
    QPushButton *Settings;
    QPushButton *FindObj;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *mem_sys;
    QLineEdit *mem_int;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QLineEdit *StartFile;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *StateRead;
    QLineEdit *StateWrite;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *ParamsRead;
    QLineEdit *ParamsWrite;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *Start;
    QPushButton *Pause;
    QPushButton *Contunue;
    QWidget *layoutWidget4;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *save_params;
    QPushButton *save_state;
    QPushButton *save_all;
    QWidget *layoutWidget5;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_12;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_6;
    QLineEdit *cpu0;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *cpu_0;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_10;
    QLineEdit *cpu1;
    QHBoxLayout *horizontalLayout_10;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *cpu_1;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_11;
    QLineEdit *cpu2;
    QHBoxLayout *horizontalLayout_11;
    QSpacerItem *horizontalSpacer_4;
    QLineEdit *cpu_2;
    QWidget *layoutWidget6;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_7;
    QLineEdit *kStep;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_8;
    QLineEdit *Accel;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_9;
    QLineEdit *Speed;
    QVBoxLayout *verticalLayout_10;
    QLabel *label_12;
    QLineEdit *Time;
    QCheckBox *Use_RSU;
    QCheckBox *Tune_Shem;
    QPushButton *Stop;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_13;
    QLineEdit *StepStop;
    QMenuBar *menubar;
    QMenu *menu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(898, 498);
        exit = new QAction(MainWindow);
        exit->setObjectName("exit");
        action = new QAction(MainWindow);
        action->setObjectName("action");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        Msg = new QLineEdit(centralwidget);
        Msg->setObjectName("Msg");
        Msg->setGeometry(QRect(30, 404, 821, 31));
        QFont font;
        font.setFamilies({QString::fromUtf8("Tahoma")});
        font.setPointSize(12);
        font.setBold(true);
        Msg->setFont(font);
        Msg->setReadOnly(true);
        Project = new QLineEdit(centralwidget);
        Project->setObjectName("Project");
        Project->setGeometry(QRect(70, 10, 621, 31));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Z003 [urw]")});
        font1.setPointSize(24);
        font1.setBold(false);
        font1.setItalic(true);
        Project->setFont(font1);
        Project->setCursor(QCursor(Qt::CursorShape::BlankCursor));
        Project->setFrame(false);
        Project->setAlignment(Qt::AlignmentFlag::AlignCenter);
        Project->setReadOnly(true);
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(20, 290, 832, 102));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        Shema = new QPushButton(layoutWidget);
        Shema->setObjectName("Shema");
        Shema->setMinimumSize(QSize(100, 100));
        Shema->setMaximumSize(QSize(100, 16777215));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Tahoma")});
        font2.setPointSize(16);
        font2.setBold(true);
        Shema->setFont(font2);

        horizontalLayout->addWidget(Shema);

        Trends = new QPushButton(layoutWidget);
        Trends->setObjectName("Trends");
        Trends->setMinimumSize(QSize(100, 100));
        Trends->setMaximumSize(QSize(100, 16777215));
        Trends->setFont(font2);

        horizontalLayout->addWidget(Trends);

        Messages = new QPushButton(layoutWidget);
        Messages->setObjectName("Messages");
        Messages->setMinimumSize(QSize(150, 100));
        Messages->setMaximumSize(QSize(150, 16777215));
        Messages->setFont(font2);

        horizontalLayout->addWidget(Messages);

        RSU = new QPushButton(layoutWidget);
        RSU->setObjectName("RSU");
        RSU->setMinimumSize(QSize(150, 100));
        RSU->setFont(font2);

        horizontalLayout->addWidget(RSU);

        Settings = new QPushButton(layoutWidget);
        Settings->setObjectName("Settings");
        Settings->setMinimumSize(QSize(150, 100));
        Settings->setMaximumSize(QSize(150, 16777215));
        Settings->setFont(font2);

        horizontalLayout->addWidget(Settings);

        FindObj = new QPushButton(layoutWidget);
        FindObj->setObjectName("FindObj");
        FindObj->setMinimumSize(QSize(150, 100));
        FindObj->setMaximumSize(QSize(100, 16777215));
        FindObj->setFont(font2);

        horizontalLayout->addWidget(FindObj);

        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(330, 60, 151, 52));
        verticalLayout_7 = new QVBoxLayout(layoutWidget1);
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName("verticalLayout_7");
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(layoutWidget1);
        label_5->setObjectName("label_5");
        label_5->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_7->addWidget(label_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        mem_sys = new QLineEdit(layoutWidget1);
        mem_sys->setObjectName("mem_sys");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(60);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mem_sys->sizePolicy().hasHeightForWidth());
        mem_sys->setSizePolicy(sizePolicy);
        mem_sys->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_6->addWidget(mem_sys);

        mem_int = new QLineEdit(layoutWidget1);
        mem_int->setObjectName("mem_int");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(40);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(mem_int->sizePolicy().hasHeightForWidth());
        mem_int->setSizePolicy(sizePolicy1);
        mem_int->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_6->addWidget(mem_int);


        verticalLayout_7->addLayout(horizontalLayout_6);

        layoutWidget2 = new QWidget(centralwidget);
        layoutWidget2->setObjectName("layoutWidget2");
        layoutWidget2->setGeometry(QRect(550, 120, 301, 110));
        verticalLayout = new QVBoxLayout(layoutWidget2);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label = new QLabel(layoutWidget2);
        label->setObjectName("label");
        label->setMinimumSize(QSize(0, 30));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Tahoma")});
        font3.setPointSize(14);
        font3.setBold(true);
        label->setFont(font3);

        horizontalLayout_4->addWidget(label);

        StartFile = new QLineEdit(layoutWidget2);
        StartFile->setObjectName("StartFile");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(20);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(StartFile->sizePolicy().hasHeightForWidth());
        StartFile->setSizePolicy(sizePolicy2);
        StartFile->setMinimumSize(QSize(0, 0));
        StartFile->setFont(font);

        horizontalLayout_4->addWidget(StartFile);

        horizontalSpacer = new QSpacerItem(90, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_2 = new QLabel(layoutWidget2);
        label_2->setObjectName("label_2");
        label_2->setMinimumSize(QSize(0, 30));
        label_2->setFont(font3);

        horizontalLayout_2->addWidget(label_2);

        StateRead = new QLineEdit(layoutWidget2);
        StateRead->setObjectName("StateRead");
        StateRead->setFont(font);
        StateRead->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_2->addWidget(StateRead);

        StateWrite = new QLineEdit(layoutWidget2);
        StateWrite->setObjectName("StateWrite");
        StateWrite->setFont(font);
        StateWrite->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_2->addWidget(StateWrite);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_3 = new QLabel(layoutWidget2);
        label_3->setObjectName("label_3");
        label_3->setMinimumSize(QSize(0, 30));
        label_3->setFont(font3);

        horizontalLayout_3->addWidget(label_3);

        ParamsRead = new QLineEdit(layoutWidget2);
        ParamsRead->setObjectName("ParamsRead");
        ParamsRead->setFont(font);
        ParamsRead->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_3->addWidget(ParamsRead);

        ParamsWrite = new QLineEdit(layoutWidget2);
        ParamsWrite->setObjectName("ParamsWrite");
        ParamsWrite->setFont(font);
        ParamsWrite->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_3->addWidget(ParamsWrite);


        verticalLayout->addLayout(horizontalLayout_3);

        layoutWidget3 = new QWidget(centralwidget);
        layoutWidget3->setObjectName("layoutWidget3");
        layoutWidget3->setGeometry(QRect(19, 190, 297, 42));
        horizontalLayout_7 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        Start = new QPushButton(layoutWidget3);
        Start->setObjectName("Start");
        Start->setMinimumSize(QSize(80, 40));
        Start->setFont(font);

        horizontalLayout_7->addWidget(Start);

        Pause = new QPushButton(layoutWidget3);
        Pause->setObjectName("Pause");
        Pause->setMinimumSize(QSize(80, 40));
        Pause->setFont(font);

        horizontalLayout_7->addWidget(Pause);

        Contunue = new QPushButton(layoutWidget3);
        Contunue->setObjectName("Contunue");
        Contunue->setMinimumSize(QSize(80, 40));
        Contunue->setFont(font);

        horizontalLayout_7->addWidget(Contunue);

        layoutWidget4 = new QWidget(centralwidget);
        layoutWidget4->setObjectName("layoutWidget4");
        layoutWidget4->setGeometry(QRect(19, 130, 254, 43));
        horizontalLayout_8 = new QHBoxLayout(layoutWidget4);
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        save_params = new QPushButton(layoutWidget4);
        save_params->setObjectName("save_params");

        horizontalLayout_8->addWidget(save_params);

        save_state = new QPushButton(layoutWidget4);
        save_state->setObjectName("save_state");

        horizontalLayout_8->addWidget(save_state);

        save_all = new QPushButton(layoutWidget4);
        save_all->setObjectName("save_all");

        horizontalLayout_8->addWidget(save_all);

        layoutWidget5 = new QWidget(centralwidget);
        layoutWidget5->setObjectName("layoutWidget5");
        layoutWidget5->setGeometry(QRect(360, 120, 181, 115));
        verticalLayout_9 = new QVBoxLayout(layoutWidget5);
        verticalLayout_9->setObjectName("verticalLayout_9");
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(layoutWidget5);
        label_4->setObjectName("label_4");
        label_4->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_9->addWidget(label_4);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName("horizontalLayout_12");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_6 = new QLabel(layoutWidget5);
        label_6->setObjectName("label_6");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(10);
        sizePolicy3.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy3);
        label_6->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(label_6);

        cpu0 = new QLineEdit(layoutWidget5);
        cpu0->setObjectName("cpu0");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(90);
        sizePolicy4.setHeightForWidth(cpu0->sizePolicy().hasHeightForWidth());
        cpu0->setSizePolicy(sizePolicy4);
        cpu0->setMinimumSize(QSize(0, 30));
        cpu0->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        verticalLayout_2->addWidget(cpu0);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        horizontalSpacer_2 = new QSpacerItem(13, 17, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_2);

        cpu_0 = new QLineEdit(layoutWidget5);
        cpu_0->setObjectName("cpu_0");
        cpu_0->setMaximumSize(QSize(30, 16777215));
        cpu_0->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_9->addWidget(cpu_0);


        verticalLayout_2->addLayout(horizontalLayout_9);


        horizontalLayout_12->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_10 = new QLabel(layoutWidget5);
        label_10->setObjectName("label_10");
        label_10->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_3->addWidget(label_10);

        cpu1 = new QLineEdit(layoutWidget5);
        cpu1->setObjectName("cpu1");
        cpu1->setMinimumSize(QSize(0, 30));
        cpu1->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        verticalLayout_3->addWidget(cpu1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        horizontalSpacer_3 = new QSpacerItem(13, 17, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_3);

        cpu_1 = new QLineEdit(layoutWidget5);
        cpu_1->setObjectName("cpu_1");
        cpu_1->setMaximumSize(QSize(30, 16777215));
        cpu_1->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_10->addWidget(cpu_1);


        verticalLayout_3->addLayout(horizontalLayout_10);


        horizontalLayout_12->addLayout(verticalLayout_3);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName("verticalLayout_8");
        label_11 = new QLabel(layoutWidget5);
        label_11->setObjectName("label_11");
        label_11->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_8->addWidget(label_11);

        cpu2 = new QLineEdit(layoutWidget5);
        cpu2->setObjectName("cpu2");
        cpu2->setMinimumSize(QSize(0, 30));
        cpu2->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        verticalLayout_8->addWidget(cpu2);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName("horizontalLayout_11");
        horizontalSpacer_4 = new QSpacerItem(13, 17, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_4);

        cpu_2 = new QLineEdit(layoutWidget5);
        cpu_2->setObjectName("cpu_2");
        cpu_2->setMaximumSize(QSize(30, 16777215));
        cpu_2->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_11->addWidget(cpu_2);


        verticalLayout_8->addLayout(horizontalLayout_11);


        horizontalLayout_12->addLayout(verticalLayout_8);


        verticalLayout_9->addLayout(horizontalLayout_12);

        layoutWidget6 = new QWidget(centralwidget);
        layoutWidget6->setObjectName("layoutWidget6");
        layoutWidget6->setGeometry(QRect(20, 60, 301, 52));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget6);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName("verticalLayout_4");
        label_7 = new QLabel(layoutWidget6);
        label_7->setObjectName("label_7");
        label_7->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_4->addWidget(label_7);

        kStep = new QLineEdit(layoutWidget6);
        kStep->setObjectName("kStep");
        kStep->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        kStep->setReadOnly(true);

        verticalLayout_4->addWidget(kStep);


        horizontalLayout_5->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName("verticalLayout_5");
        label_8 = new QLabel(layoutWidget6);
        label_8->setObjectName("label_8");
        label_8->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_5->addWidget(label_8);

        Accel = new QLineEdit(layoutWidget6);
        Accel->setObjectName("Accel");
        Accel->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        Accel->setReadOnly(true);

        verticalLayout_5->addWidget(Accel);


        horizontalLayout_5->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName("verticalLayout_6");
        label_9 = new QLabel(layoutWidget6);
        label_9->setObjectName("label_9");
        label_9->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_6->addWidget(label_9);

        Speed = new QLineEdit(layoutWidget6);
        Speed->setObjectName("Speed");
        Speed->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        verticalLayout_6->addWidget(Speed);


        horizontalLayout_5->addLayout(verticalLayout_6);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setObjectName("verticalLayout_10");
        label_12 = new QLabel(layoutWidget6);
        label_12->setObjectName("label_12");
        label_12->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_10->addWidget(label_12);

        Time = new QLineEdit(layoutWidget6);
        Time->setObjectName("Time");
        Time->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        verticalLayout_10->addWidget(Time);


        horizontalLayout_5->addLayout(verticalLayout_10);

        Use_RSU = new QCheckBox(centralwidget);
        Use_RSU->setObjectName("Use_RSU");
        Use_RSU->setGeometry(QRect(20, 250, 131, 22));
        Tune_Shem = new QCheckBox(centralwidget);
        Tune_Shem->setObjectName("Tune_Shem");
        Tune_Shem->setGeometry(QRect(180, 250, 131, 22));
        Stop = new QPushButton(centralwidget);
        Stop->setObjectName("Stop");
        Stop->setGeometry(QRect(759, 10, 121, 40));
        Stop->setMinimumSize(QSize(80, 40));
        Stop->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/monitor.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Stop->setIcon(icon);
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(310, 250, 141, 27));
        horizontalLayout_13 = new QHBoxLayout(widget);
        horizontalLayout_13->setObjectName("horizontalLayout_13");
        horizontalLayout_13->setContentsMargins(0, 0, 0, 0);
        label_13 = new QLabel(widget);
        label_13->setObjectName("label_13");
        label_13->setMaximumSize(QSize(100, 16777215));
        label_13->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_13->addWidget(label_13);

        StepStop = new QLineEdit(widget);
        StepStop->setObjectName("StepStop");
        StepStop->setMaximumSize(QSize(40, 16777215));
        StepStop->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_13->addWidget(StepStop);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 898, 22));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menu->addAction(exit);
        menu->addAction(action);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        exit->setText(QCoreApplication::translate("MainWindow", "\320\267\320\260\320\272\320\276\320\275\321\207\320\270\321\202\321\214", nullptr));
        action->setText(QCoreApplication::translate("MainWindow", "\320\267\320\260\320\263\321\200\320\272\320\267\320\270\321\202\321\214", nullptr));
        Shema->setText(QCoreApplication::translate("MainWindow", "\320\241\321\205\320\265\320\274\321\213", nullptr));
        Trends->setText(QCoreApplication::translate("MainWindow", "\320\242\321\200\320\265\320\275\320\264\321\213", nullptr));
        Messages->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\320\276\320\261\321\211\320\265\320\275\320\270\321\217", nullptr));
        RSU->setText(QCoreApplication::translate("MainWindow", "\320\240\320\241\320\243", nullptr));
        Settings->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
        FindObj->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\270\321\201\320\272", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\320\237\320\260\320\274\321\217\321\202\321\214", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\241\321\202\320\260\321\200\321\202\320\276\320\262\321\213\320\271 \321\204\320\260\320\271\320\273", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "          \320\241\320\276\321\201\321\202\320\276\321\217\320\275\320\270\320\265", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "         \320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213", nullptr));
        Start->setText(QCoreApplication::translate("MainWindow", "\320\237\321\203\321\201\320\272", nullptr));
        Pause->setText(QCoreApplication::translate("MainWindow", "\320\237\320\260\321\203\320\267\320\260", nullptr));
        Contunue->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\276\320\264\320\276\320\273\320\266\320\270\321\202\321\214", nullptr));
        save_params->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214\n"
"\320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213", nullptr));
        save_state->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214\n"
"\321\201\320\276\321\201\321\202\320\276\321\217\320\275\320\270\320\265", nullptr));
        save_all->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214\n"
"\320\262\321\201\320\265", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\276\321\206\320\265\321\201\321\201\320\276\321\200\321\213", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\321\217\320\264\321\200\320\276", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "\320\274\320\276\320\264", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "\320\263\320\270\320\264\321\200\320\260", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\320\250\320\260\320\263\320\270", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\320\243\321\201\320\272\320\276\321\200\320\265\320\275\320\270\320\265", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "\320\222\321\200\320\265\320\274\321\217", nullptr));
        Use_RSU->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\260\321\202\321\214 \320\240\320\241\320\243", nullptr));
        Tune_Shem->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\264\320\263\320\276\320\275\320\272\320\260 \321\201\321\205\320\265\320\274", nullptr));
        Stop->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\272\320\276\320\275\321\207\320\270\321\202\321\214", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "\320\250\320\260\320\263 \320\276\321\201\321\202\320\260\320\275\320\276\320\262\320\260", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
