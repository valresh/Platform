QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
LIBS += -L$$DESTDIR$$ -lACS -lIntReg
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
SOURCES += \
Motor.cpp\
Motor_Calc.cpp\
Motor_Conn.cpp\
Motor_Defects.cpp\
Motor_dll.cpp\
Motor_Param.cpp\
Oborot.cpp

HEADERS  += \
ACS.h\
Motor.h\
Motor_Defects.h\
Node.h\
Points.h
