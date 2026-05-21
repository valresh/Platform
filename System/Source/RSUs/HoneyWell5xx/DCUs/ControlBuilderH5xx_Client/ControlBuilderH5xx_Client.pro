QT += core gui
QT += widgets

CONFIG += c++17

TEMPLATE = lib

DEFINES += LINUX

INCLUDEPATH += $$PWD/../../../../Include

DESTDIR= "$$PWD/../../../../../EXE"

HEADERS += \
  CBsPool.h \
  H_RemoteClient.h \
  ICBsPool.h \
  stdafx.h

SOURCES += \
  ACSsupport.cpp \
  CBsPool.cpp \
  ControlBuilderH5xx_Client.cpp \
  H_RemoteClient.cpp \
  RsuConnectionImpl.cpp \
  stdafx.cpp

