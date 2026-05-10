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
Level.cpp\
Level_Calc.cpp\
Level_Defects.cpp\
Level_dll.cpp\
Level_Param.cpp

HEADERS  += \
ACS.h\
Level.h\
Points.h
