QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lChem -lEpsVol -lIntReg -lData -lUniHeat
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
SepLiq.cpp\
SepLiq_Calc.cpp\
SepLiq_Defects.cpp\
SepLiq_dll.cpp\
SepLiq_Hydro.cpp\
SepLiq_Param.cpp\
Water.cpp

HEADERS  += \
ACS.h\
Points.h\
SepLiq.h\
SepLiq_Defects.h
