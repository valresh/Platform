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
  QB_UserDefName.h\
  LocalUtils.h\
  ServerExSharedPlugin_Honey.h\
  stdafx.h\
  ../../../Version/DCUVers.rc\
  EventStringH.h
SOURCES += \
  alarmsext.cpp\
  FindConnPointEx.cpp\
  FindHoney.cpp\
  LocalUtils.cpp\
  PutModel.cpp\
  ServerExSharedPlugin_Honey_dll.cpp\
  stdafx.cpp\
  EventStringH.cpp
