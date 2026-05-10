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
BaseCol_dll.cpp\
ColAdditions.cpp\
Column.cpp\
ColumnSub.cpp\
Eps.cpp\
Pressure.cpp\
Show.cpp\
Stage.cpp\
StageAdd.cpp\
StageSub.cpp\
Output.cpp\
Karman.cpp\
Cond_In_Vol.cpp\
Cond_In_Sloj.cpp\
Common.cpp\
Temperature.cpp

HEADERS  += \
Description.h
