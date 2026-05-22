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
  Client.h\
  HcdPool.h\
  IHcdPool.h
SOURCES += \
  HCD_Client_dll.cpp\
  stdafx.cpp\
  ACSsupport.cpp\
  Client.cpp\
  RsuConnectionImpl.cpp\
  HcdPool.cpp
