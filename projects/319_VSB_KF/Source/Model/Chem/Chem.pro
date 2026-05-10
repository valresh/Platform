QT       += core

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
QMAKE_CXXFLAGS_WARN_OFF += -Wwrite-strings\
-Wattributes

#HEADERS  += 

HEADERS += \
  Macro.h \
  stdafx.h

SOURCES += \
  Appr.cpp \
  Appr_Cp.cpp \
  Chem.cpp \
  ChemFunc.cpp \
  Chem_dll.cpp \
  CompList.cpp \
  ISOData.cpp \
  ISOIdeal.cpp \
  IdealMix.cpp \
  MixVol.cpp \
  Pseudo.cpp \
  Real.cpp \
  Solid.cpp \
  WaterNIST.cpp
