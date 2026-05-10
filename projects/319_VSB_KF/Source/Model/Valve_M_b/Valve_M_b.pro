QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lIntReg -lValve_b
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
V.cpp\
Valve_M_b.cpp\
Valve_M_b_Calc.cpp\
Valve_M_b_Defects.cpp\
Valve_M_b_dll.cpp\
Valve_M_b_Param.cpp\
VEP.cpp\
VEP_Defects.cpp\
VEP_Hydro.cpp\
VEP_Param.cpp\
V_Calc.cpp\
V_Defects.cpp\
V_Hydro.cpp\
V_Param.cpp

HEADERS  += \
V.h\
Valve_M_b.h\
Valve_M_b_ACS.h\
Valve_M_b_Defects_Pnt.h\
Valve_M_b_Pnt.h\
VEP.h\
VEP_ACS.h\
VEP_Defects.h\
Vep_Pnt.h\
V_ACS.h\
V_Defects.h\
V_Pnt.h
