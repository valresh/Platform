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
  stdafx.h\
  ../../../Version/DCUVers.rc\
  Alarms.h
SOURCES += \
  AlarmsHW.cpp\
  stdafx.cpp\
  Alarms.cpp
