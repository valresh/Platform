QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Electro.cpp\
Electro_Calc.cpp\
Electro_Defects.cpp\
Electro_dll.cpp\
Electro_Param.cpp

HEADERS  += \
ACS.h\
Electro.h\
Points.h
