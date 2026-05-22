QT += core gui
QT += widgets

CONFIG += c++17

TEMPLATE = lib

DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../../Common/Include\
$$PWD/../../Include


DESTDIR= $$PWD/../../../../../EXE
HEADERS += \
  resource.h\
  ServerTest.h\
  stdafx.h\
  ServerTest.rc
SOURCES += \
  ServerTest.cpp\
  stdafx.cpp
