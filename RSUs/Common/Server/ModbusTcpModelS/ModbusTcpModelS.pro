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
  MbPointConfig.h\
  ModbusTcpServerImpl.h\
  ModbusTcpHandle.h\
  ../../Include/TinyXmlHelper.h\
  ../../Include/TinyXmlReader.h\
  IPool.h\
  Pool.h
SOURCES += \
  ModbusTcpModelS_dll.cpp\
  stdafx.cpp\
  ACSsupport.cpp\
  ModbusTcpServer_ConfigRead.cpp\
  ModbusTcpServer_Work.cpp\
  ModbusTcpServerImpl.cpp\
  WndProc.cpp\
  WndThread.cpp\
  ModbusTcpHandle.cpp\
  Pool.cpp
