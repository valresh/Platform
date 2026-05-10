QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lChem -lEpsVol -lIntReg -lData -lUniHeat  -lACS
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
HeatAntiFlow.cpp\
HeatAntiFlow_Calc.cpp\
HeatAntiFlow_Defects.cpp\
HeatAntiFlow_dll.cpp\
HeatAntiFlow_Hydro.cpp\
HeatAntiFlow_Param.cpp

HEADERS  += \
ACS.h\
HeatAntiFlow.h\
Points.h
