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
  AlarmKernalHW5xx.h\
  AlarmsFile.h\
  AlarmsW.h\
  AlmGr.h\
  Mnemo.h\
  ..\..\..\Common\Include\TinyXmlHelper.h\
  ..\..\..\Common\Include\TinyXmlReader.h
SOURCES += \
  AlarmKernalHW5xx_dll.cpp\
  stdafx.cpp\
  AlarmKernalHW5xx.cpp\
  AlarmsFile.cpp\
  AlarmsW.cpp\
  AlmGr.cpp\
  Mnemo.cpp
