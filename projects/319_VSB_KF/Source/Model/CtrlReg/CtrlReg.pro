QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
DEFINES += LINUX\
DLL_CTRLREG_EXPORTS\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
CtrlReg.cpp\
CtrlReg_Defects.cpp\
CtrlReg_dll.cpp\
CtrlReg_Param.cpp

HEADERS  += \
ACS.h\
Points.h
