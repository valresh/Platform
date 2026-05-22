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
  stdafx.h\
  TrendsRsuKernel.h\
  ../../Include/TrendSrvH.h\
  ../../Include/TrendStr.h
SOURCES += \
  stdafx.cpp\
  TrendsKernel_dll.cpp\
  TrendsRsuKernel-File.cpp\
  TrendsRsuKernel-Fill.cpp\
  TrendsRsuKernel.cpp
