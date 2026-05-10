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
CC_1.cpp\
CC_1_Calc.cpp\
CC_1_Defects.cpp\
CC_1_dll.cpp\
CC_1_Hydro.cpp\
CC_1_Param.cpp\
Stage.cpp\
Thermo.cpp

HEADERS  += \
ACS.h\
CC_1.h\
CC_1_Defects.h\
ComprStage.h\
Points.h\
Points_Stage.h
