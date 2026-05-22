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
  TrendsRemoteClient.h
SOURCES += \
  stdafx.cpp\
  TrendsRSU_Client.cpp\
  FillArray.cpp\
  LoadPlugins.cpp\
  TrendsRemoteClient.cpp
