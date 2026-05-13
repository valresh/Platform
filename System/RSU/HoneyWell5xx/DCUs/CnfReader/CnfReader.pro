##QT -= gui\
 QT += widgets

TEMPLATE = lib
#CONFIG += staticlib

CONFIG += c++17
QMAKE_CXXFLAGS_RELEASE -= -O2

INCLUDEPATH += $$PWD/../../../../../System/Source/Include \
$$PWD/../../Include \
$$PWD/../../../../system/include/rsus \
$$PWD/../../../../system \
$$PWD/../../../system \
$$PWD/../../../common/include

DESTDIR= "$$PWD/../../../EXE"
#LIBS += -L$$DESTDIR$$ -lRSUutils -lzlib

DEFINES += LINUX\
MODELS\
CNFREADERQ_DECL_EXPORTORTS\
COMMONRSUSQ_DECL_EXPORTORTS\
CNFREADER_API\
UTILSH5XXQ_DECL_EXPORTORTS\
X64

HEADERS += \
    Block.h \
    CnfReader.h \
    Connection.h \
    Coord.h \
    IBlock.h \
    ISymbolAttr.h \
    SymbolAttr.h \
    Vertex.h

SOURCES += \
    Block.cpp \
    CnfReader.cpp \
    Coord.cpp \
    SymbolAttr.cpp \
    Vertex.cpp

DISTFILES +=



