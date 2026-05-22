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
  ../../Include/HCD/8DI.h\
  ../../Include/HCD/AI.h\
  ../../Include/HCD/AO.h\
  ../../Include/HCD/DO.h\
  ../../Include/HCD/AnalogSignal.h\
  ../../Include/HCD/AnalogVar.h\
  ../../Include/HCD/DigitalSignal.h\
  ../../Include/HCD/DigitalVar.h\
  ../../Include/HCD_Declare.h\
  HCD_Sheme.h\
  ../../Include/HCDBaseType_.h\
  ../../Include/HcdDataTypes_.h\
  ../../Include/HcdShData.h\
  ../../Include/HcdStruct.h\
  ../../Include/HCDType.hpp\
  ListValue.h\
  stdafx.h\
  ../../Include/UtilsHCD.h
SOURCES += \
  BuildList.cpp\
  HCDStruct.cpp\
  ListValue.cpp\
  stdafx.cpp\
  UtilsHCD.cpp
