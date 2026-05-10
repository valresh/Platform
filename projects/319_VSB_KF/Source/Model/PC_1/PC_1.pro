QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lChem -lEpsVol -lData -lUniHeat
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Logica.cpp\
Oborot.cpp\
PC_1.cpp\
PC_1_Calc.cpp\
PC_1_Defects.cpp\
PC_1_dll.cpp\
PC_1_Hydro.cpp\
PC_1_Param.cpp\
Stage.cpp\
Thermo.cpp

HEADERS  += \
ACS.h\
Node.h\
PC_1.h\
PC_1_Defects.h\
Points.h
