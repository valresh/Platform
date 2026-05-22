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
  ServerRsuTcp.h\
  stdafx.h\
  _ttached.ico\
  ServerRsuTcp.rc\
  ClientConnection.h\
  ServerImpl.h\
  TrendReaderImpl.h\
  TrendServer.h
SOURCES += \
  ../../../../SYSTEM/Source/Utils/Utils/LogFile.cpp\
  ServerRsuTcp.cpp\
  stdafx.cpp\
  ClientConnection.cpp\
  ServerImpl.cpp\
  TrendReaderImpl.cpp\
  TrendServer.cpp
