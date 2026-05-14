QT += core gui
QT += widgets

CONFIG += c++17

TEMPLATE = lib

DEFINES += EMF_DLL

INCLUDEPATH += $$PWD/../Include

DESTDIR= "$$PWD/../../../EXE"

SOURCES += \
    EMF.cpp \
    main.cpp \
    Bitmap.cpp \
    BitmapHeader.cpp \
    EmfHeader.cpp \
    EmfLogger.cpp \
    EmfObjects.cpp \
    EmfOutput.cpp \
    EmfParser.cpp \
    EmfRecords.cpp \
    QEmfRenderer.cpp

HEADERS += \
    ../../Include/EMF.h \
    ../Include/EMF.h \
    Bitmap.h \
    BitmapHeader.h \
    EmfEnums.h \
    EmfHeader.h \
    EmfLogger.h \
    EmfObjects.h \
    EmfOutput.h \
    EmfParser.h \
    EmfRecords.h \
    QEmfRenderer.h

FORMS += \
  EMF.ui

DEFINES += EMF_DLL

#INCLUDEPATH += \
#    $$PWD/libqemf/src

