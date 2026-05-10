QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lChem -lEpsVol -lControl -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Collector.cpp\
Collector_Calc.cpp\
Collector_Defects.cpp\
Collector_dll.cpp\
Collector_Hydro.cpp\
Collector_Param.cpp

HEADERS  += \
ACS.h\
Collector.h\
Collector_Defects.h\
Points.h
