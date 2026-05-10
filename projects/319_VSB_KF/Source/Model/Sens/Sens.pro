QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lExtensions -lACS -lChem -lEpsVol -lControl -lData -lTBP
DEFINES += LINUX\
X64

TARGET = "Sens_b"
INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Sens.cpp\
Sens_Calc.cpp\
Sens_Defects.cpp\
Sens_dll.cpp\
Sens_Hydro.cpp\
Sens_Param.cpp\
Analys.cpp

HEADERS  += \
ACS.h\
CtrlVar.h\
Node.h\
Points.h\
Sens.h
