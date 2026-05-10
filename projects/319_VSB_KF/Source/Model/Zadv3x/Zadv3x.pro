QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Zadv3x.cpp\
Zadv3x_Calc.cpp\
Zadv3x_Defects.cpp\
Zadv3x_dll.cpp\
Zadv3x_Hydro.cpp\
Zadv3x_Param.cpp

HEADERS  += \
ACS.h\
Points.h\
Zadv3x.h
