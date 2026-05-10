QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lExtensions -lACS -lChem -lData -lBaseCol_NE
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
2x2.cpp\
Column.cpp\
Column_Calc.cpp\
Column_Defects.cpp\
Column_dll.cpp\
Column_Hydro.cpp\
Column_Param.cpp\
Extensions.cpp\
Fast.cpp\
H2SCO2.cpp\
Matr.cpp\
Pressure.cpp\
ReadColData.cpp\
Static.cpp

HEADERS  += \
2x2.h\
ACS.h\
Column_NE.h\
H2SCO2.h\
Points.h
