QT += core gui
QT += widgets

CONFIG += c++17

TEMPLATE = lib

DEFINES += LINUX\
X64

INCLUDEPATH += $$PWD/../../../Common/Include\
$$PWD/../../Include


DESTDIR= $$PWD/../../../../../EXE
HEADERS += \
  BaseInt.h \
  stdafx.h\
  AssignRsuPointsVals.h
SOURCES += \
  AssignRsuPointsVals_dll.cpp\
  stdafx.cpp\
  AssignRsuPointsVals.cpp
