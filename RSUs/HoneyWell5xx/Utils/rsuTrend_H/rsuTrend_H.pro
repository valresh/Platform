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
  rsuTrend_H.h\
  stdafx.h\
  ../../../Version/DCUVers.rc
SOURCES += \
  rsuTrend_H.cpp\
  stdafx.cpp
