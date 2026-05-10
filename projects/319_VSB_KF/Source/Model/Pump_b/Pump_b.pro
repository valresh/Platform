QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lChem -lEpsVol -lIntReg -lData -lMotor -lCoolObj -lHeatOil
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Dynamic.cpp\
Dynamic_Calc.cpp\
Dynamic_Defects.cpp\
Dynamic_Hydro.cpp\
Dynamic_Param.cpp\
Pump_b.cpp\
Pump_b_Calc.cpp\
Pump_b_Defects.cpp\
Pump_b_dll.cpp\
Pump_b_Electro.cpp\
Pump_b_Hydro.cpp\
Pump_b_Param.cpp\
Volumetric.cpp\
Volumetric_Calc.cpp\
Volumetric_Defects.cpp\
Volumetric_Hydro.cpp\
Volumetric_Param.cpp

HEADERS  += \
Dynamic.h\
Dynamic_ACS.h\
Dynamic_Defects.h\
Dynamic_Pnt.h\
Pump_b.h\
Pump_b_ACS.h\
Volumetric.h\
Volumetric_ACS.h\
Volumetric_Defects.h\
Volumetric_Pnt.h
