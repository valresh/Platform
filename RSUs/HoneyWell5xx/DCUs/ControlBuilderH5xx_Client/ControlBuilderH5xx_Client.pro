QT += core gui
QT += widgets

CONFIG += c++17

TEMPLATE = lib

DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../../Common/Include\
$$PWD/../../Include


DESTDIR= "$$PWD/../../../../../EXE"

HEADERS += \
  BaseType.h \
  BaseType.hpp \
  CBase.h \
  CBsPool.h \
  Connect.h \
  H_RemoteClient.h \
  ICBsPool.h \
  RsuX.h \
  RsuX_types.h \
  SetDataTypes.h \
  Str.h \
  stdafx.h

SOURCES += \
  ACSsupport.cpp \
  CBsPool.cpp \
  ControlBuilderH5xx_Client.cpp \
  H_RemoteClient.cpp \
  RsuConnectionImpl.cpp \
  stdafx.cpp

