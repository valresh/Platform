QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
DEFINES += LINUX\
X64
LIBS += -L$$DESTDIR$$ -lSysUtils

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
ACS.cpp\
ACScomponent.cpp\
ACS_dll.cpp\
analog.cpp\
diskrete.cpp

HEADERS  += \
ACScomponent.h
