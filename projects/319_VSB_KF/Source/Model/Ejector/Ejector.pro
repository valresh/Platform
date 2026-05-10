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
Ejector.cpp\
Ejector_Calc.cpp\
Ejector_Defects.cpp\
Ejector_dll.cpp\
Ejector_Hydro.cpp\
Ejector_Param.cpp

HEADERS  += \
ACS.h\
Ejector.h\
Ejector_Defects.h\
Points.h
