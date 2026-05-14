QT += widgets

TEMPLATE = lib
DEFINES += RSUUTILS_LIBRARY

CONFIG += c++17

DESTDIR= "$$PWD/../../EXE"

INCLUDEPATH += $$PWD/../../System/Source/Include \
$$PWD/../../Include \
$$PWD/../../../../system/include/rsus \
$$PWD/../../../../system \
$$PWD/../system \
$$PWD/../common/include

SOURCES += \
    ../common/utils/CommonRSUs/cutf.cpp \
    ../common/utils/CommonRSUs/rsuErr.cpp \
    ../common/utils/CommonRSUs/rsuStr.cpp \
    ../common/utils/TinyXML2/tinyxml2.cpp

HEADERS +=

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
