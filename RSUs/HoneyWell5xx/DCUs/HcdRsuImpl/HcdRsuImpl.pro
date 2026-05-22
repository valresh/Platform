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
  HcdRsuImpl_dll.h\
  stdafx.h\
  ..\..\Include\HCDBaseType_.h\
  ..\..\Include\HcdDataTypes_.h\
  ..\..\Include\HcdVarTypes.h\
  ..\..\Include\IHcdDrawable.h\
  ..\..\..\Version\DCUVers.rc\
  HcdRsu.h\
  HcdConnection.h\
  HcdPage.h\
  HcdSheet.h\
  ..\..\Include\HCD\IStateSer.h\
  StateSer.h\
  ..\..\..\Common\Include\TinyXmlHelper.h\
  ..\..\..\Common\Include\TinyXmlReader.h
SOURCES += \
  HcdRsuImpl_dll.cpp\
  stdafx.cpp\
  HcdRsu.cpp\
  HcdRsu_ACS.cpp\
  HcdRsu_Calc.cpp\
  HcdRsu_Init.cpp\
  HcdRsu_ParamsSR.cpp\
  HcdRsu_SRstate.cpp\
  HcdConnection.cpp\
  HcdPage.cpp\
  HcdSheet.cpp\
  StateSer.cpp
