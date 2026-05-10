QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lLS -lSysUtils
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
CreateStruct.cpp\
CtrlConn.cpp\
CtrlConn_Calc.cpp\
CtrlConn_Defects.cpp\
CtrlConn_dll.cpp\
CtrlConn_Param.cpp\
ReadMMCsv.cpp\
ReadRsuCsv.cpp\
SensDefects.cpp\
Stable.cpp\
Syntax.cpp\
Translate.cpp\
Utils.cpp

HEADERS  += \
ACS.h\
CtrlConn.h\
Node.h\
PntConnects.h\
Points.h\
Trans.h
