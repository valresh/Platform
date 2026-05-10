QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
CalcGroup.cpp\
Common.cpp\
Full_Lib.cpp\
Group.cpp\
Load.cpp\
Object.cpp\
Points.cpp\
Pool.cpp\
UniHydro.cpp\
UniHydro_dll.cpp\
UniHydro_Param.cpp

HEADERS  += \
ACS.h\
Full_Lib.h\
HydroCtrl.h\
Points.h\
UniHydro.h
