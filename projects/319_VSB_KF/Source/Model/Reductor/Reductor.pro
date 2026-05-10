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
Reductor.cpp\
Reductor_Calc.cpp\
Reductor_Defects.cpp\
Reductor_dll.cpp\
Reductor_Hydro.cpp\
Reductor_Param.cpp

HEADERS  += \
ACS.h\
Points.h\
Reductor.h
