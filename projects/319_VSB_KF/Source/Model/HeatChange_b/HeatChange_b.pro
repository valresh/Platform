QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lChem -lEpsVol -lIntReg -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Calc_1F.cpp\
Calc_2F.cpp\
Cp.cpp\
HeatChange.cpp\
HeatChange_Calc.cpp\
HeatChange_Defects.cpp\
HeatChange_dll.cpp\
HeatChange_Hydro.cpp\
HeatChange_Param.cpp\
Q.cpp\
UniHeatChange.cpp

HEADERS  += \
ACS.h\
HeatChange.h\
HeatChange_Defects.h\
Points.h\
UniHeatChange.h
