QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

DESTDIR= "$$PWD/../../../EXE"

LIBS += -L$$DESTDIR$$ -lSysUtils -lEMFReader -lzlib

INCLUDEPATH +=  $$PWD/../Include


DEFINES += LINUX

QMAKE_CFLAGS += -Wl,--stack= 100000000

SOURCES += \
  DCU.cpp \
    Msg/msglist.cpp \
  RSU/RSU.cpp \
  RSU/Start_0.cpp \
   Sheme/findobj.cpp \
    Sheme/listshem.cpp \
    Sheme/showparams.cpp \
    Sheme/showsheme.cpp \
  Start.cpp \
    Trends/listtrends.cpp \
    Trends/minitrend.cpp \
    Trends/qtrends.cpp \
    Trends/showtrends.cpp \
 #   Trends/trends.cpp \
    Trends/trendsvar.cpp \
    Trends/trendvarprop.cpp \
    Utils.cpp \
    Work.cpp \
    list.cpp \
    main.cpp \
    mainwindow.cpp \
    paramlist.cpp \
    qlistliewmouse.cpp \
    qtableviewmouse.cpp \
    showrsu.cpp \
    winlist.cpp

HEADERS += \
    ../../Include/dialcolor.h \
    ../../Include/list.h \
    ../../Include/qtrends.h \
    ../Include/dialcolor.h \
    ../Include/list.h \
    Msg/msglist.h \
  RSU/RSU.h \
  RSU/fs_path.h \
  Sheme/findobj.h \
    Sheme/listshem.h \
    Sheme/showparams.h \
    Sheme/showsheme.h \
    Start.h \
    Trends/listtrends.h \
    Trends/minidraw.h \
    Trends/minitrend.h \
    Trends/qtrends.h \
    Trends/showtrends.h \
#    Trends/trends.h \
    Trends/trendsvar.h \
    Trends/trendvarprop.h \
    clistview.h \
    list.h \
    listemf.h \
    mainwindow.h \
    minidraw.h \
    paramslist.h \
    paramstree.h \
    qlistliewmouse.h \
    qtableviewmouse.h \
    showrsu.h

FORMS += \
    Msg/msglist.ui \
  Sheme/findobj.ui \
    Sheme/listshem.ui \
    Sheme/showparams.ui \
    Trends/minitrend.ui \
    Trends/qtrends.ui \
  #  Trends/trends.ui \
    Trends/trendsvar.ui \
    Trends/trendvarprop.ui \
    list.ui \
    listtrends.ui \
    mainwindow.ui \
  showrsu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
  Icons.qrc

DISTFILES +=
