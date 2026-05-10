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
Valve_b.cpp\
Valve_b_Calc.cpp\
Valve_b_Defects.cpp\
Valve_b_dll.cpp\
Valve_b_Hydro.cpp\
Valve_b_Param.cpp

HEADERS  += \

