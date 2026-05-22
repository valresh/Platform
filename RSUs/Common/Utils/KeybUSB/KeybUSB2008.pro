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
  Ioctls.h\
  KbdDriver.h\
  ../../Include/KeybCOM.h\
  Keyboard.h\
  Resource.h\
  stdafx.h\
  YokoUSB.h\
  Keyboard.bmp\
  KeybUSB.ico\
  KeybUSB.rc\
  small.ico\
  small1.ico\
  PipeScreen.h\
  PipeSerS.h\
  PipeUprawl.h
SOURCES += \
  AboutUSB.cpp\
  KbdDriver.cpp\
  Keyboard.cpp\
  KeybUSB.cpp\
  ../../../../SYSTEM/Source/Utils/Utils/LogFile.cpp\
  stdafx.cpp\
  YokoUSB.cpp\
  PipeS.cpp\
  PipeScreen.cpp\
  PipeSerS.cpp\
  PipeUprawl.cpp
