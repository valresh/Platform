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
  ..\..\Include\QuickBuilder\Analog.h\
  ..\..\Include\QuickBuilder\Analog_P.h\
  ParamShow.h\
  ..\..\Include\QuickBuilder\Psa.h\
  ..\..\Include\QuickBuilder\Psa_P.h\
  ..\..\Include\QuickBuilder\Status.h\
  ..\..\Include\QuickBuilder\Status_P.h\
  ListValue.h\
  ..\..\Include\QB_Declare.h\
  QB_Sheme.h\
  ..\..\Include\QbBaseType_.h\
  ..\..\Include\QbBridge2SysParam.h\
  ..\..\Include\QbDataTypes_.h\
  ..\..\Include\QbShData.h\
  ..\..\Include\QuickBuilderStruct.h\
  ..\..\Include\QuickBuilderType.hpp\
  stdafx.h\
  ..\..\Include\UtilsQB.h\
  ..\..\..\Version\DCUVers.rc
SOURCES += \
  BuildList.cpp\
  ListValue.cpp\
  QuickBuilderStruct.cpp\
  stdafx.cpp\
  UtilsQB.cpp\
  ANALOG.cpp\
  PSA.cpp\
  STATUS.cpp
