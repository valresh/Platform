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
  ParamShow.h\
  ../../Include/SafetyManager/SmAI.h\
  ../../Include/SafetyManager/SmAI_P.h\
  ../../Include/SafetyManager/SmAO.h\
  ../../Include/SafetyManager/SmAO_P.h\
  ../../Include/SafetyManager/SmBI.h\
  ../../Include/SafetyManager/SmBI_P.h\
  ../../Include/SafetyManager/SmBICOM.h\
  ../../Include/SafetyManager/SmBICOM_P.h\
  ../../Include/SafetyManager/SmBO.h\
  ../../Include/SafetyManager/SmBO_P.h\
  ../../Include/SafetyManager/SmBOCOM.h\
  ../../Include/SafetyManager/SmBOCOM_P.h\
  ../../Include/SafetyManager/SmDI.h\
  ../../Include/SafetyManager/SmDI_P.h\
  ../../Include/SafetyManager/SmDO.h\
  ../../Include/SafetyManager/SmDO_P.h\
  ../../Include/SafetyManager/SmDOCOM.h\
  ../../Include/SafetyManager/SmDOCOM_P.h\
  ../../Include/SafetyManager/SmFld.h\
  ../../Include/SafetyManager/SmFld_P.h\
  ../../Include/SafetyManager/SmAnalog.h\
  ../../Include/SafetyManager/SmAnalog_P.h\
  ../../Include/SafetyManager/SmDiscret.h\
  ../../Include/SafetyManager/SmDiscret_P.h\
  ListValue.h\
  ../../Include/SafetyManagerStruct.h\
  ../../Include/SafetyManagerType.hpp\
  ../../Include/SM_Declare.h\
  SM_Sheme.h\
  ../../Include/SmBaseType_.h\
  ../../Include/SmBridge2SysParam.h\
  ../../Include/SmDataTypes_.h\
  ../../Include/SmShData.h\
  stdafx.h\
  ../../Include/UtilsSM.h\
  ../../../Version/DCUVers.rc
SOURCES += \
  BuildList.cpp\
  ListValue.cpp\
  SafetyManagerStruct.cpp\
  stdafx.cpp\
  UtilsSM.cpp\
  ANALOG.cpp\
  DISCRET.cpp\
  BICOM.cpp\
  BOCOM.cpp\
  FLD.cpp\
  SmAI.cpp\
  SmAO.cpp\
  SmBI.cpp\
  SmBO.cpp\
  SmDI.cpp\
  SmDO.cpp\
  SmDOCOM.cpp
