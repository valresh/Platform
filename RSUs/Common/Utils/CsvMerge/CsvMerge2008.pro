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
  ChildView.h\
  CsvMerge.h\
  MainFrm.h\
  Resource.h\
  stdafx.h\
  res/CsvMerge.ico\
  CsvMerge.rc\
  res/CsvMerge.rc2\
  res/Toolbar.bmp
SOURCES += \
  ChildView.cpp\
  CsvMerge.cpp\
  MainFrm.cpp\
  stdafx.cpp\
  Uralkaliy.cpp
