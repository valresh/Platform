QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lEpsVol -lIntReg -lData
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
ElectricHeater_b.cpp\
ElectricHeater_b_Calc.cpp\
ElectricHeater_b_Defects.cpp\
ElectricHeater_b_dll.cpp\
ElectricHeater_b_Electro.cpp\
ElectricHeater_b_Hydro.cpp\
ElectricHeater_b_Param.cpp

HEADERS  += \
ACS.h\
ElectricHeater_b.h\
ElectricHeater_b_Defects.h\
Points.h
