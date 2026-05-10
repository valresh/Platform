QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lChem -lControl -lData -lBaseCol
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Column.cpp\
Column_Calc.cpp\
Column_Defects.cpp\
Column_dll.cpp\
Column_Hydro.cpp\
Column_Param.cpp

HEADERS  += \
ACS.h\
Column.h\
Column_Defects.h\
Points.h
