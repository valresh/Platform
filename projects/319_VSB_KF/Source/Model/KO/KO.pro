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
KO.cpp\
KO_Calc.cpp\
KO_Calc_N.cpp\
KO_Defects.cpp\
KO_dll.cpp\
KO_Hydro.cpp\
KO_Hydro_N.cpp\
KO_Param.cpp

HEADERS  += \
ACS.h\
KO.h\
Points.h
