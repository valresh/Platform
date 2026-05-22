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
  GuiH.def\
  MyDocManager.h\
  MyMultiDocTemplate.h\
  Resource.h\
  stdafx.h\
  ..\..\..\Version\DCUVers.rc\
  GuiH.rc\
  res\GuiH.rc2
SOURCES += \
  GuiH.cpp\
  MyDocManager.cpp\
  MyMultiDocTemplate.cpp\
  stdafx.cpp
