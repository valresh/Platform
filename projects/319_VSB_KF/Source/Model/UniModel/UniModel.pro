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
UniModel.cpp\
UniModel_Calc.cpp\
UniModel_Defects.cpp\
UniModel_dll.cpp\
UniModel_Hydro.cpp\
UniModel_Param.cpp\
Drossel.cpp\
Luk.cpp\
Sprey.cpp\
Dryer.cpp\
Recycle.cpp\
Composition.cpp\
ElectroHeat.cpp

HEADERS  += \
ACS.h\
Points.h\
UniModel.h
