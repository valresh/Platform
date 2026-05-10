QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lExtensions -lACS -lChem -lEpsVol -lData
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
RadTube.cpp

HEADERS  += \
ACS.h\
Furnace.h\
Points.h
