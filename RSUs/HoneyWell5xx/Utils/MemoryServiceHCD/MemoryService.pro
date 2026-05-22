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
  ..\..\..\Version\DCUVers.rc\
  ..\..\Include\HcdMemoryService.h
SOURCES += \
  MemoryService_dll.cpp\
  stdafx.cpp\
  HcdMemoryService.cpp
