QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lChem -lEpsVol -lControl -lIntReg -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Control.cpp\
IO.cpp\
IO_Calc.cpp\
IO_Defects.cpp\
IO_dll.cpp\
IO_Hydro.cpp\
IO_Param.cpp\
Matr.cpp

HEADERS  += \
ACS.h\
IO.h\
Node.h\
Points.h
