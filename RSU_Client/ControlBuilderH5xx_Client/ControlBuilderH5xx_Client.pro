QT += core gui
QT += widgets

CONFIG += c++17

TEMPLATE = lib

DEFINES += LINUX

INCLUDEPATH += $$PWD/../../System/Source/Include

DESTDIR= "$$PWD/../../EXE"

#INCLUDEPATH += \
#    $$PWD/libqemf/src

HEADERS += \
  BaseType_.h \
  CBsPool.h \
  DataTypes_.h \
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

