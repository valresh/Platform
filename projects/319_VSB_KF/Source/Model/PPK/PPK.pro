QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
PPK.cpp\
PPK_Calc.cpp\
PPK_Defects.cpp\
PPK_dll.cpp\
PPK_Hydro.cpp\
PPK_Param.cpp

HEADERS  += \
ACS.h\
Points.h\
PPK.h\
PPK_Defects.h
