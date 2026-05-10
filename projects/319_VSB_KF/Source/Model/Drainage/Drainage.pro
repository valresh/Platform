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
Drainage.cpp\
Drainage_Calc.cpp\
Drainage_Defects.cpp\
Drainage_dll.cpp\
Drainage_Hydro.cpp\
Drainage_Param.cpp

HEADERS  += \
ACS.h\
Drainage.h\
Drainage_Defects.h\
Node.h\
Points.h
