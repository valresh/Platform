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
  Resource.h\
  RsuServer.h\
  stdafx.h\
  RsuServer.ico\
  RsuServer.rc\
  small.ico\
  MainWindow.h\
  ../../Include/rsuIPC.h\
  ../../Include/rsuModel.h\
  IModelHandler.h\
  ModelHandler.h
SOURCES += \
  RsuServer.cpp\
  stdafx.cpp\
  MainWindow.cpp\
  ../../../../SYSTEM/Source/Utils/Utils/LogFile.cpp\
  ModelHandler.cpp
