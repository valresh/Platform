QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lChem -lData -lChemSpec
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Base.cpp\
ComponentControl.cpp\
Eps.cpp\
EpsVol_dll.cpp\
Func.cpp\
Vol.cpp\
Vol_2.cpp

HEADERS  += \
ComponentControl.h
