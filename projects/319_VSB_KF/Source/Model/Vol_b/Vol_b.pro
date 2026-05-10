QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lExtensions -lACS -lChem -lControl -lIntReg -lData -lVolBase_b -lUniHeat
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Extensions.cpp\
Vol.cpp\
Vol_Calc.cpp\
Vol_Defects.cpp\
Vol_dll.cpp\
Vol_Hydro.cpp\
Vol_Param.cpp

HEADERS  += \
ACS.h\
Node.h\
Points.h\
Vol.h\
Vol_Defects.h
