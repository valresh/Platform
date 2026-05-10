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
2Zm.cpp\
AntiFlow.cpp\
AntiFlowCool.cpp\
Cool.cpp\
Cooler.cpp\
Heat.cpp\
Heat_1_flow.cpp\
Heat_2_flow.cpp\
TubeVol.cpp\
UniHeat_dll.cpp

HEADERS  += \

