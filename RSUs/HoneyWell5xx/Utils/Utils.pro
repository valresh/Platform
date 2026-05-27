QT += widgets

TEMPLATE = subdirs

BUILDDIR = $$PWD/../../../SYSTEM/TEMP/UniHydro

OBJECTS_DIR = $${BUILDDIR}
DEFINES += LINUX

DESTDIR= $$PWD/../../../../../EXE

INCLUDEPATH += $$PWD/../../../Common/Include\
$$PWD/../../Include

QMAKE_CXXFLAGS_WARN_OFF += -Wwrite-strings\
-Wattributes

SUBDIRS += \
   CommonH
#   ConnectH\
#   ConnectHCD\
#   ConnectQB\
#   ConnectSM\
#   HCDRegisterInRSU\
#   HW5xxRegisterInRSU\
#   HW5xxRegisterInSystem\
#   MemoryServiceH5xx\
#   MemoryServiceHCD\
#   MemoryServiceQB5xx\
#   MemoryServiceSM5xx\
#   QB5xxRegisterInRSU\
##   rsuTrend_H\
##   ServerExPlugin_Honey\
##   ServerExSharedPlugin_Honey\
#   SM5xxRegisterInRSU
#   UtilsH\
#   UtilsHCD\
#   UtilsQB\
#   UtilsSM
