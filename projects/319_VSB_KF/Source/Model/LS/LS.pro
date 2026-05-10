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
LS.cpp\
LS_Calc.cpp\
LS_Defects.cpp\
LS_dll.cpp\
LS_Param.cpp

HEADERS  += \
ACS.h\
LS.h\
Points.h
