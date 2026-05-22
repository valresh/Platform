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
  H_RemoteClient.h\
  CBsPool.h\
  ICBsPool.h
SOURCES += \
  ControlBuilderH5xx_Client.cpp\
  stdafx.cpp\
  ACSsupport.cpp\
  H_RemoteClient.cpp\
  RsuConnectionImpl.cpp\
  CBsPool.cpp
