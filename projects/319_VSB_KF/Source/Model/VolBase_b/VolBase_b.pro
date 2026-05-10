QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lChem -lEpsVol -lNewComp -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
DynamicMix.cpp\
Eps.cpp\
F2.cpp\
Init.cpp\
Level.cpp\
OLD.cpp\
Params.cpp\
StaticMix.cpp\
StaticT.cpp\
SummFlow.cpp\
Utils.cpp\
VolBase.cpp\
VolBase_dll.cpp\
_Dynamic.cpp\
_Static.cpp

HEADERS  += \

