QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lChem
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
ALL_Data.cpp\
ALL_Data_Param.cpp\
ComponentControl.cpp\
Data.cpp\
Data_dll.cpp\
Extension.cpp\
Save_Rest.cpp\
Solid.cpp

HEADERS  += \
ALL_Data.h\
ComponentControl.h
