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
  ServerExPlugin_Honey.h\
  stdafx.h\
  ..\..\..\Version\DCUVers.rc\
  EventStringH.h
SOURCES += \
  alarmsext.cpp\
  PutModel.cpp\
  ServerExPlugin_Honey_dll.cpp\
  stdafx.cpp\
  EventStringH.cpp
