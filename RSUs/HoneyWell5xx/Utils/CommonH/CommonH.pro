QT += core gui
QT += widgets

CONFIG += c++17

TEMPLATE = lib

DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../../Common/Include\
$$PWD/../../Include


DESTDIR= $$PWD/../../../../EXE
HEADERS += \
  ../../Include/CommonH.h\
  stdafx.h
SOURCES += \
  CommonH.cpp\
  CommonH_dll.cpp\
  stdafx.cpp
