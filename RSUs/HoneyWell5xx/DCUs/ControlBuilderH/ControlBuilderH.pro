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
  ../../Include/ConnectionMB.h\
  ../../../Common/Include/macros/en2str.h\
  ../../Include/HoneywellStruct.h\
  ../../Include/HoneywellType.hpp\
  ../../Include/HPARM_INIT.h\
  ParmVarInfo.h\
  Resource.h\
  stateKeys.h\
  stdafx.h\
  ../../../Common/Include/varhelper.h\
  ../../Include/WP_Declare.h\
  ../../../Common/Include/xmlhelper.h\
  res/bmp161.bmp\
  ControlBuilder.rc\
  ../../../Version/DCUVers.rc\
  CBPool.h\
  ControlBuilder.h\
  ../../Include/IControlBuilder.h\
  AlarmTestPV.h\
  ../../Include/BmBase.h\
  ../../Include/H_Class.h\
  ../../Include/H_Data.h\
  ../../Include/HBridge2SysParam.h\
  ../../Include/CalcRes.h\
  ../../Include/CalcVar.h\
  ../../Include/transtypes.h\
  ../../../Common/Include/TinyXmlHelper.h\
  ../../../Common/Include/TinyXmlReader.h
SOURCES += \
  ControlBuilder_dll.cpp\
  ParmVarInfo.cpp\
  stdafx.cpp\
  CBPool.cpp\
  ControlBuilder.cpp\
  ControlBuilder_ACS.cpp\
  ControlBuilder_Calc.cpp\
  ControlBuilder_Init.cpp\
  ControlBuilder_Link.cpp\
  ControlBuilder_Params.cpp\
  ControlBuilder_State.cpp\
  SaveRestoreProject.cpp\
  ConnectionMB.cpp\
  BlockCreate.cpp\
  BmBase.cpp\
  H_Block.cpp\
  H_Data.cpp\
  ModuleCreate.cpp\
  PCDIFLAGARRCH.cpp\
  PCDINUMARRCH.cpp\
  RBPERMISSION.cpp\
  AICHANNEL.cpp\
  AOCHANNEL.cpp\
  DICHANNEL.cpp\
  DOCHANNEL.cpp\
  SPARE.cpp\
  2OO3.cpp\
  AND.cpp\
  CHECKBAD.cpp\
  DELAY.cpp\
  EQ.cpp\
  FTRIG.cpp\
  GE.cpp\
  GT.cpp\
  LE.cpp\
  LIMIT.cpp\
  LT.cpp\
  MAX.cpp\
  MAXPULSE.cpp\
  MIN.cpp\
  MINPULSE.cpp\
  MUXREAL.cpp\
  NAND.cpp\
  NE.cpp\
  NOR.cpp\
  NOT.cpp\
  OFFDELAY.cpp\
  ONDELAY.cpp\
  OR.cpp\
  PULSE.cpp\
  QOR.cpp\
  ROR.cpp\
  RS.cpp\
  RTRIG.cpp\
  SEL.cpp\
  SELREAL.cpp\
  SR.cpp\
  STARTSIGNAL.cpp\
  TRIG.cpp\
  WATCHDOG.cpp\
  XOR.cpp\
  ABS.cpp\
  ADD.cpp\
  DIV.cpp\
  MUL.cpp\
  NEG.cpp\
  ROLLAVG.cpp\
  SUB.cpp\
  DIGACQ.cpp\
  FIRSTOUT.cpp\
  FLAG.cpp\
  FLAGARRAY.cpp\
  MESSAGE.cpp\
  NUMERIC.cpp\
  NUMERICARRAY.cpp\
  PUSH.cpp\
  TEXTARRAY.cpp\
  TEXTCOMMENT.cpp\
  TIMER.cpp\
  TYPECONVERT.cpp\
  AUXCALC.cpp\
  AUXCALC_FP.cpp\
  AUXSUMMER.cpp\
  CTUD.cpp\
  DATAACQ.cpp\
  DEADTIME.cpp\
  DEVCTL.cpp\
  ENHAUXCALC.cpp\
  FLOWCOMP.cpp\
  GENLIN.cpp\
  SIGNALSEL.cpp\
  TOTALIZER.cpp\
  ScriptEng/CalcRes.cpp\
  ScriptEng/CalcVar.cpp\
  AUTOMAN.cpp\
  ENHREGCALC.cpp\
  FANOUT.cpp\
  OVRDSEL.cpp\
  PID.cpp\
  PIDFF.cpp\
  RATIOBIAS.cpp\
  RATIOCTL.cpp\
  REGCALC.cpp\
  REGSUMMER.cpp\
  SWITCH.cpp\
  848_0601.cpp\
  MAI.cpp\
  9413-DIGIT_0102.AI.cpp\
  9413-DIGIT_0102.DO1TB.cpp\
  9413-DIGIT_0102.MDI.cpp\
  9413-DIGIT_0102.MDO.cpp\
  9413-DIGIT_0102.RESOURCE.cpp\
  PICHANNEL.cpp\
  PITOTALIZER.cpp\
  TC-MDP081.cpp\
  AIREF.cpp\
  AOREF.cpp\
  DIREF.cpp\
  DOREF.cpp\
  BOOL2NUM.cpp\
  NUM2BOOL.cpp\
  INTERLOCK.cpp\
  STEP.cpp\
  TRANSITION.cpp\
  HAICHANNEL.cpp\
  SP_AI.cpp\
  SP_AO.cpp\
  SP_DI.cpp\
  SP_DO.cpp\
  SP_SPDVOTE.cpp\
  SP_SPEED.cpp\
  AI-LLMUX.cpp\
  C300.cpp\
  CEEC300.cpp\
  CEEFB.cpp\
  CONTROLMODULE.cpp\
  CPM200.cpp\
  Dx_24y.cpp\
  Dx_SOE.cpp\
  FFLINK.cpp\
  FIM4.cpp\
  FTEB.cpp\
  IOLINK.cpp\
  PCDI_MASTER.cpp\
  RMFBCLASS.cpp\
  Root.cpp\
  TC-IAH161.cpp\
  TC-IDD321.cpp\
  TC-IXL061.cpp\
  TC-IXL062.cpp\
  TC-IXR061.cpp\
  TC-OAV081.cpp\
  TC-ODD321.cpp\
  xHART_HL.cpp\
  9413_DIGIT_0102.cpp\
  HANDLER.cpp\
  SCM.cpp\
  AO16.cpp\
  DI.cpp\
  DI24V.cpp\
  DISOE.cpp\
  DO32.cpp\
  HLAI.cpp\
  HLAIHART.cpp\
  LLMUX.cpp\
  UIO-2.cpp\
  UIO.cpp\
  SP.cpp
