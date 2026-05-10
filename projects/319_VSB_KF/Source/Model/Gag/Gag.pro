QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Gag.cpp\
Gag_Calc.cpp\
Gag_Defects.cpp\
Gag_dll.cpp\
Gag_Hydro.cpp\
Gag_Param.cpp

HEADERS  += \
ACS.h\
Gag.h\
Gag_Defects.h\
Points.h
