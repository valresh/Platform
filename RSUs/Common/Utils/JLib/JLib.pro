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
  CJ60Lib.def\
  CJ60Lib.rc\
  CJControlBar.h\
  CJDockBar.h\
  CJDockContext.h\
  CJFlatButton.h\
  CJFlatComboBox.h\
  CJFrameWnd.h\
  CJMiniDockFrameWnd.h\
  CJSizeDockBar.h\
  CJTabCtrlBar.h\
  CJToolBar.h\
  FixTB.h\
  FlatBar.h\
  Globals.h\
  ModulVer.h\
  StdAfx.h\
  res/btn_arro.bmp\
  res/btn_explorer.bmp\
  res/button_images.bmp\
  res/CJ60Lib.rc2\
  res/cj_logo.bmp\
  res/hsplitba.cur\
  res/vsplitba.cur
SOURCES += \
  CJ60lib.cpp\
  CJControlBar.cpp\
  CJDockBar.cpp\
  CJDockContext.cpp\
  CJFlatButton.cpp\
  CJFlatComboBox.cpp\
  CJFrameWnd.cpp\
  CJMiniDockFrameWnd.cpp\
  CJSizeDockBar.cpp\
  CJTabCtrlBar.cpp\
  CJToolBar.cpp\
  FixTB.cpp\
  FlatBar.cpp\
  Globals.cpp\
  ModulVer.cpp\
  StdAfx.cpp
