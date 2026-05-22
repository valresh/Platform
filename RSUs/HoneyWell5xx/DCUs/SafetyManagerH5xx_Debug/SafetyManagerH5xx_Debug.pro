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
  resource.h\
  SafetyManagerDebug_dll.h\
  stdafx.h\
  ..\..\..\Version\DCUVers.rc\
  res\folders.bmp\
  res\handtrackyes.cur\
  SafetyManagerH5xx_Debug.rc\
  EditFix.h\
  SafetyManagerApp.h\
  SafetyManagerDoc.h\
  SafetyManagerFrame.h\
  SafetyManagerView.h\
  ViewList.h\
  CbhList.h\
  ComList.h\
  FldComList.h\
  FldList.h
SOURCES += \
  SafetyManagerDebug_dll.cpp\
  stdafx.cpp\
  EditFix.cpp\
  SafetyManagerApp.cpp\
  SafetyManagerDoc.cpp\
  SafetyManagerFrame.cpp\
  SafetyManagerView.cpp\
  ViewList.cpp\
  CbhList.cpp\
  ComList.cpp\
  FldComList.cpp\
  FldList.cpp
