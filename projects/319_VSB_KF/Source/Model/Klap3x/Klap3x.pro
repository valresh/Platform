QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lChem -lEpsVol -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Klap3x.cpp\
Klap3x_Calc.cpp\
Klap3x_Defects.cpp\
Klap3x_dll.cpp\
Klap3x_Hydro.cpp\
Klap3x_Param.cpp

HEADERS  += \
ACS.h\
Klap3x.h\
Klap3x_Defects.h\
Points.h
