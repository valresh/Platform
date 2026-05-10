QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lChem -lEpsVol -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
VBR.cpp\
VBR_Calc.cpp\
VBR_Defects.cpp\
VBR_dll.cpp\
VBR_Hydro.cpp\
VBR_Param.cpp

HEADERS  += \
ACS.h\
Node.h\
Points.h\
VBR.h
