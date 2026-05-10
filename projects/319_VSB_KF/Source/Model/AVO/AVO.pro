QT       += core gui

CONFIG += c++17

DESTDIR= "$$PWD/../../../../../EXE"

TEMPLATE = lib

LIBS += -L$$DESTDIR$$ -lACS -lChem -lEpsVol -lControl -lIntReg -lData -lUniHeat -lSysUtils

DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include

SOURCES += \
AVO.cpp\
AVO_Calc.cpp\
AVO_Defects.cpp\
AVO_dll.cpp\
AVO_Hydro.cpp\
AVO_Param.cpp

HEADERS  += \
ACS.h\
AVO.h\
AVO_Defects.h\
Points.h
