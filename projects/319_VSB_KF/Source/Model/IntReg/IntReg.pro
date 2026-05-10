QT       += core gui

CONFIG += c++17
DESTDIR= "$$PWD/../../../../../EXE"
TEMPLATE = lib
DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include
#SOURCES += 

#HEADERS  += 

SOURCES += \
  IntReg.cpp \
  SetComp.cpp

