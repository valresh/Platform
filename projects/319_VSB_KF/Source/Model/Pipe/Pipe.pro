QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lExtensions -lChem -lEpsVol -lControl -lIntReg -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
CatPipe.cpp\
Pipe.cpp\
Pipe_Calc.cpp\
Pipe_Defects.cpp\
Pipe_dll.cpp\
Pipe_Hydro.cpp\
Pipe_Param.cpp\
Pool.cpp

HEADERS  += \
ACS.h\
Pipe.h\
Pipe_Defects.h\
Points.h
