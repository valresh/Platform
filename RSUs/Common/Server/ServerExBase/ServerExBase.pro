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
  Resource.h\
  ServerExBase.h\
  stdafx.h\
  ..\..\..\Version\DCUVers.rc\
  ServerExBase.rc\
  MemBuffer.h\
  ..\..\Include\ServerExPluginTypes.h\
  ServerThreadBase.h\
  MessageList.h\
  SMessage.h\
  TrendFile.h\
  TrendReadCycle.h\
  ..\..\Include\TrendsCommon.h
SOURCES += \
  ..\..\..\..\SYSTEM\Source\Utils\Utils\LogFile.cpp\
  ServerExBase.cpp\
  stdafx.cpp\
  MnemoToModel.cpp\
  ModelToMnemo.cpp\
  PluginsSup.cpp\
  ServerThreadBase.cpp\
  ”правление моделью.cpp\
  MessageList.cpp\
  PutModelLocal.cpp\
  SMessage.cpp\
  TrendFile.cpp\
  TrendReadCycle.cpp
