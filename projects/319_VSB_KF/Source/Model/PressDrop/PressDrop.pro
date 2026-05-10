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
PressDrop.cpp\
PressDrop_Calc.cpp\
PressDrop_dll.cpp\
PressDrop_Param.cpp

HEADERS  += \
ACS.h
