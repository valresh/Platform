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
  ../Include/HoneyMnemoExtend.h\
  stdafx.h
SOURCES += \
  HoneyMnemoExtend.cpp\
  HoneyMnemoExtend_dll.cpp\
  stdafx.cpp
