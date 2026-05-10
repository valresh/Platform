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
Classes.cpp\
Connect.cpp\
Script.cpp\
Script_dll.cpp\
Script_Param.cpp\
UserScript.cpp

HEADERS  += \
Points.h\
Script.h\
Common.h
