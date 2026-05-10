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
Analog.cpp\
Control.cpp\
Control_Calc.cpp\
Control_dll.cpp\
Control_Param.cpp\
Discrete.cpp\
Level.cpp\
Omega.cpp\
ShowValue.cpp\
Stable.cpp

HEADERS  += \
Points.h
