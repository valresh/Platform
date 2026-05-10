QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lChem -lEpsVol -lData -lHeatOil
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Fan.cpp\
Fan_Calc.cpp\
Fan_Defects.cpp\
Fan_dll.cpp\
Fan_Electro.cpp\
Fan_Hydro.cpp\
Fan_Param.cpp\
Oborot.cpp

HEADERS  += \
ACS.h\
Fan.h\
Fan_Defects.h\
Points.h
