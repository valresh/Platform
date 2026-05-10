QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lChem -lEpsVol -lNewComp -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Fors.cpp\
ForsLogic.cpp\
Furnace.cpp\
Furnace_Calc.cpp\
Furnace_Defects.cpp\
Furnace_dll.cpp\
Furnace_Hydro.cpp\
Furnace_Param.cpp\
Radiation.cpp\
RadTube.cpp\
UB.cpp

HEADERS  += \
ACS.h\
Furnace.h\
Furnace_Defects.h\
Node.h\
Points.h
