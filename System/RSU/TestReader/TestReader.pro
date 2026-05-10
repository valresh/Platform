QT += widgets

#CONFIG += staticlib

CONFIG += c++17
QMAKE_CXXFLAGS_RELEASE -= -O2

DESTDIR= "$$PWD/../../../../EXE"

SOURCES += \
        main.cpp

RESOURCES += qml.qrc

