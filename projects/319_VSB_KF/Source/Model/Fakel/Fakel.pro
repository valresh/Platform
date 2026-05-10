QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lChem -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Fakel.cpp\
Fakel_Calc.cpp\
Fakel_Defects.cpp\
Fakel_dll.cpp\
Fakel_Hydro.cpp\
Fakel_Param.cpp

HEADERS  += \
ACS.h\
Fakel.h\
Points.h
