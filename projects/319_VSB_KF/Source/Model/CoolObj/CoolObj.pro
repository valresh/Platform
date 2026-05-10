QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lEpsVol -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
CoolObj.cpp\
CoolObj_Calc.cpp\
CoolObj_Defects.cpp\
CoolObj_dll.cpp\
CoolObj_Hydro.cpp\
CoolObj_Param.cpp

HEADERS  += \
ACS.h\
CoolObj.h\
CoolObj_Defects.h\
Node.h\
Points.h
