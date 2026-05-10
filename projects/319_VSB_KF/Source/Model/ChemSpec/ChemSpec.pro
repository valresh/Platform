QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lChem -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
C2H4.cpp\
C5H10.cpp\
CaCl2.cpp\
ChemSpec.cpp\
ChemSpec_dll.cpp\
ChemSpec_Param.cpp\
Cyclobutadiene.cpp\
  IAPWS/0.cpp \
  IAPWS/1.cpp \
  IAPWS/2.cpp \
  IAPWS/3.cpp \
  IAPWS/4.cpp \
  IAPWS/5.cpp \
  IAPWS/Appr.cpp \
  IAPWS/Func.cpp \
  IAPWS/Water.cpp \
  IAPWS/Water_appr.cpp \
  ISO/ISOData.cpp \
  ISO/ISOIdeal.cpp \
MAN.cpp\
  MDEA/MDEA_CO2.cpp \
  MDEA/MDEA_H2S.cpp \
  MDEA/MEA_CO2.cpp \
Mw.cpp\
  NH3/NH3_Appr.cpp \
  NH3/NH3_H2O.cpp \
  NH3/NH3_H2O_Appr.cpp \
  NH3/NH3_Helmgolz.cpp \
Pseudo.cpp\
  SPEC/COKE.cpp \
  SPEC/O.cpp \
  SPEC/Sulfur.cpp \
SolidData.cpp\
SpecialData.cpp


HEADERS  += \
ChemSpec.h\
  IAPWS/IAPWS.h \
  IAPWS/stdafx.h \
  ISO/ISO.h \
Points.h
