QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
MultiSens.cpp\
MultiSens_Calc.cpp\
MultiSens_Defects.cpp\
MultiSens_dll.cpp\
MultiSens_Param.cpp\
Oborot.cpp

HEADERS  += \
ACS.h\
MultiSens.h\
MultiSens_Defects.h\
Node.h\
Points.h
