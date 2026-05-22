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
  QB_RemoteClient.h
SOURCES += \
  QuickBuilderH5xx_Client.cpp\
  stdafx.cpp\
  ACSsupport.cpp\
  QB_RemoteClient.cpp\
  RsuConnectionImpl.cpp
