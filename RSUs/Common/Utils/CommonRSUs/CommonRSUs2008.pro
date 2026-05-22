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
  ../../Include/CommonRSUs.h\
  ../../Include/hashRSU.h\
  stdafx.h
SOURCES += \
  CommonRSUs.cpp\
  CommonRSUs_dll.cpp\
  convert_utf8_to_windows1251.cpp\
  hashRSU.cpp\
  stdafx.cpp
