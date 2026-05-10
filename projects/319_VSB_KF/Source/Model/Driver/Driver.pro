QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Driver.cpp\
#Driver_Calc.cpp\
Driver_Defects.cpp\
Driver_dll.cpp\
Driver_Hydro.cpp\
Driver_Param.cpp\
Pool.cpp

HEADERS  += \
ACS.h\
Driver.h\
Points.h
