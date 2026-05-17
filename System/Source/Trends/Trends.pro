QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20


DESTDIR= "$$PWD/../../../EXE"

#LIBS += -L$$DESTDIR$$ -lSysUtils  -lzlib

INCLUDEPATH +=  $$PWD/../Include

DEFINES += LINUX\
UTILS_EXPORTS\
X64

SOURCES += \
    Connect.cpp \
    DB.cpp \
    Err.cpp \
    Files.cpp \
    Str.cpp \
    Trends.cpp \
    dialcolor.cpp \
    listtrends.cpp \
    main.cpp \
    maintrends.cpp \
    qtableviewmouse.cpp \
    qtrends.cpp \
    showtrends.cpp \
    sysutils.cpp \
    trendsvar.cpp \
    trendvarprop.cpp \
    winlist.cpp

HEADERS += \
    dialcolor.h \
    listtrends.h \
    maintrends.h \
    qtableviewmouse.h \
    qtrends.h \
    showtrends.h \
    trendsvar.h \
    trendvarprop.h

FORMS += \
    maintrends.ui \
    qtrends.ui \
    trendsvar.ui \
    trendvarprop.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
  Icons.qrc

