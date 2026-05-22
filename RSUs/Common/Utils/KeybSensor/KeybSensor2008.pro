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
  ..\..\Include\KeybCOM.h\
  Keyboard.h\
  Resource.h\
  stdafx.h\
  YokoUSB.h\
  ..\..\..\Version\DCUVers.rc\
  KeybHW.ico\
  KeybHW.rc\
  small.ico\
  small1.ico\
  PipeScreen.h\
  PipeSerS.h\
  PipeUprawl.h
SOURCES += \
  AboutUSB.cpp\
  Keyboard.cpp\
  KeybSensor.cpp\
  stdafx.cpp\
  UnknUSB.cpp\
  YokoUSB.cpp\
  PipeS.cpp\
  PipeScreen.cpp\
  PipeSerS.cpp\
  PipeUprawl.cpp
