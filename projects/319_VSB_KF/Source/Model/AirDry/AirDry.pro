QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lChem -lEpsVol -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
AirDry.cpp\
AirDry_Calc.cpp\
AirDry_Defects.cpp\
AirDry_dll.cpp\
AirDry_Hydro.cpp\
AirDry_Param.cpp

HEADERS  += \
ACS.h\
AirDry.h\
AirDry_Defects.h\
Points.h
