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
  ServerExPlugin_Model.h\
  ..\..\..\Common\Include\ServerExPluginTypes.h\
  stdafx.h\
  ..\..\..\Version\DCUVers.rc
SOURCES += \
  MessagesExt.cpp\
  ServerExPlugin_Model_dll.cpp\
  stdafx.cpp
