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
  QBPARM_INIT.h\
  stdafx.h\
  ..\..\Include\QB_Declare.h\
  ..\..\Include\QbBaseType_.h\
  ..\..\Include\QbDataTypes_.h\
  ..\..\Include\QbShData.h\
  ..\..\Include\QuickBuilderStruct.h\
  ..\..\Include\QuickBuilderType.hpp\
  ..\..\Include\QuickBuilder\Analog.h\
  ..\..\Include\QuickBuilder\Analog_P.h\
  ..\..\Include\QuickBuilder\Status.h\
  ..\..\Include\QuickBuilder\Status_P.h\
  ..\..\..\Version\DCUVers.rc\
  QuickBuilder.h\
  stateKeys.h\
  ParmVarInfo.h\
  QB_Class.h\
  QbBase.h\
  QBParamInfo.h\
  QbRsuX.h\
  QbTranslator.h\
  Trans.h\
  CalcVar.h\
  ScriptHandle.h
SOURCES += \
  QuickBuilderH.cpp\
  stdafx.cpp\
  QuickBuilder.cpp\
  QuickBuilder_Calc.cpp\
  QuickBuilder_Init.cpp\
  QuickBuilder_ReadProject.cpp\
  QuickBuilder_SaveRestoreProject.cpp\
  QuickBuilder_State.cpp\
  RsuConnectionImpl.cpp\
  ParmVarInfo.cpp\
  QbBase.cpp\
  QBParamInfo.cpp\
  QbRsuX.cpp\
  QB_ANALOG.cpp\
  QB_PSA.cpp\
  QB_STATUS.cpp\
  AnalizeTok.cpp\
  Func.cpp\
  GetTokens.cpp\
  Prior.cpp\
  QbTranslator.cpp\
  Var.cpp\
  CalcVar.cpp\
  ScriptHandle.cpp
