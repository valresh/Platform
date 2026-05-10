QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lData -lPressDrop
DEFINES += LINUX\
X64

TARGET = "Valve_R_b"

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Valve_R.cpp\
Valve_R_Calc.cpp\
Valve_R_Defects.cpp\
Valve_R_dll.cpp\
Valve_R_Hydro.cpp\
Valve_R_Param.cpp

HEADERS  += \
ACS.h\
Points.h\
Valve_R.h\
Valve_R_Defects.h
