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
  AssignWvarToVariable.h\
  AutoCloser.h\
  emfEx.h\
  FBDsConverter.h\
  FBDsConverterDoc.h\
  FBDsConverterView.h\
  Find.h\
  MainFrm.h\
  ParsingHelp.h\
  Resource.h\
  SheetsView.h\
  spl.h\
  stdafx.h\
  res\FBDsConverter.ico\
  FBDsConverter.rc\
  res\FBDsConverter.rc2\
  res\FBDsConverterDoc.ico\
  res\Toolbar.bmp\
  GraphObjPoints.h\
  IGraphObjPoints.h\
  BaseReader.h\
  IParameterReader.h\
  ReadParamsFromEMF.h\
  4ADD.h\
  8DI.h\
  8DO.h\
  ABS.h\
  ADD.h\
  AI.h\
  ALM.h\
  AND.h\
  AO.h\
  ASYS.h\
  CAVG.h\
  CMPR.h\
  DENC.h\
  DI.h\
  DIV.h\
  DLAY.h\
  DO.h\
  FRCP.h\
  HLLM.h\
  HMATH.h\
  HMON.h\
  HSEL.h\
  LMON.h\
  LTCH.h\
  MBR.h\
  MBS.h\
  MBW.h\
  MDFL.h\
  MDSW.h\
  MUL.h\
  NOT.h\
  OFDT.h\
  ONDT.h\
  OR.h\
  PID.h\
  PTMR.h\
  RK.h\
  ROC.h\
  RSTAT.h\
  RSW.h\
  RTC.h\
  RTMR.h\
  SCB.h\
  STRIG.h\
  SUB.h\
  SW.h\
  TCPR.h\
  TCPS.h\
  TGFF.h\
  TOT.h\
  TPSC.h\
  TRIG.h\
  TRND.h\
  TRPT.h\
  UPDN.h\
  WVAR.h\
  ISignalTags.h\
  SignalTags.h\
  Variable.h\
  CppGenerator.h\
  Graph2Logix.h\
  Graph2Logix_data.h\
  ParsGraph.h\
  Trim.h\
  OpenXmlExport.h
SOURCES += \
  AssignWvarToVariable.cpp\
  FBDsConverter.cpp\
  FBDsConverterDoc.cpp\
  FBDsConverterDoc2.cpp\
  FBDsConverterView.cpp\
  Find.cpp\
  MainFrm.cpp\
  ParsingHelp.cpp\
  SheetsView.cpp\
  stdafx.cpp\
  GraphObjPoints.cpp\
  AllSignalTagsVariablesReader.cpp\
  BaseReader.cpp\
  BlockParametersReader.cpp\
  ReadParamsFromEMF.cpp\
  SignalTagsReader.cpp\
  VariablesReader.cpp\
  8DI.cpp\
  8DO.cpp\
  AI.cpp\
  ALM.cpp\
  AND.cpp\
  AO.cpp\
  CAVG.cpp\
  DI.cpp\
  DLAY.cpp\
  DO.cpp\
  HLLM.cpp\
  HMATH.cpp\
  HMON.cpp\
  LMON.cpp\
  MBR.cpp\
  MBS.cpp\
  MBW.cpp\
  OFDT.cpp\
  ONDT.cpp\
  OR.cpp\
  PID.cpp\
  PTMR.cpp\
  ROC.cpp\
  RTMR.cpp\
  SCB.cpp\
  STRIG.cpp\
  TCPR.cpp\
  TCPS.cpp\
  TOT.cpp\
  TPSC.cpp\
  UPDN.cpp\
  WVAR.cpp\
  SignalTags.cpp\
  Variable.cpp\
  CppGenerator.cpp\
  Graph2Logix.cpp\
  ParsGraph.cpp\
  OpenXmlExport.cpp
