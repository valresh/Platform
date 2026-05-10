QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lChem -lEpsVol -lNewComp -lData -lUniHeat -lChemSpec
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include

SOURCES += \
BaseCol_dll.cpp\
Column.cpp\
ColumnSub.cpp\
Eps.cpp\
Stage.cpp\
StageAdd.cpp\
StageSub.cpp\
Vol.cpp

HEADERS  += \

