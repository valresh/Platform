QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DESTDIR= "$$PWD/../../EXE"

LIBS += -L$$DESTDIR$$ -lCnfReader -lRSUutils -lzlib
# -lSysUtils


INCLUDEPATH += $$PWD/../../System/Source/Include \
$$PWD/../../Include \
$$PWD/../../../../system/include/rsus \
$$PWD/../../../../system \
$$PWD/../system \
$$PWD/../common/include

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
