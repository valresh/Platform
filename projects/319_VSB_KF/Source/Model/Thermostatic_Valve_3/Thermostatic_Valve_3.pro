QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lChem -lEpsVol -lData -lChemSpec
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Thermostatic_Valve_3.cpp\
Thermostatic_Valve_3_Calc.cpp\
Thermostatic_Valve_3_Defects.cpp\
Thermostatic_Valve_3_dll.cpp\
Thermostatic_Valve_3_Hydro.cpp\
Thermostatic_Valve_3_Param.cpp

HEADERS  += \
ACS.h\
Points.h\
Thermostatic_Valve_3.h
