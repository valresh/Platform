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
  ../../Include/CreateStructHook.h\
  stdafx.h
SOURCES += \
  CreateStructHook.cpp\
  stdafx.cpp
