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
Temp.cpp\
Temp_Calc.cpp\
Temp_Defects.cpp\
Temp_dll.cpp\
Temp_Hydro.cpp\
Temp_Param.cpp

HEADERS  += \
Points.h\
Temp.h
