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
  H_Server.h
SOURCES += \
  ControlBuilderH5xx_Server_dll.cpp\
  stdafx.cpp\
  H_Server.cpp\
  RsuConnectionImpl.cpp\
  H_Params.cpp\
  H_States.cpp
