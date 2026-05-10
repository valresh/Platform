##QT -= gui\
 QT += widgets

TEMPLATE = lib
#CONFIG += staticlib

CONFIG += c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
INCLUDEPATH +=  $$PWD/../Include

DESTDIR= "$$PWD/../../../EXE"

DEFINES += LINUX\
MODELS\
UTILS_EXPORTS\
X64

SOURCES += \
    BaseModel.cpp \
    Connect.cpp \
    DB.cpp \
    Defects.cpp \
    Err.cpp \
    Extern_Pnt.cpp \
    Files.cpp \
    GDI.cpp \
    Load.cpp \
    Mes.cpp \
    ModelDll.cpp \
    Param.cpp \
    Prop.cpp \
    PropsLoader.cpp \
    ReadParams.cpp \
    ReadState.cpp \
    RsuX/ConnectionsCont.cpp \
    RsuX/RSU_Params.cpp \
    RsuX/RsuX.cpp \
    RsuX/RsuX_types.cpp \
    RsuX/RSU.cpp \
    SR.cpp \
    SortTree.cpp \
    Str.cpp \
    Struct/ExternalCondition.cpp \
    Struct/Flags.cpp \
    Struct/Forsunka.cpp \
    Struct/GasAnalyz.cpp \
    Struct/HS.cpp \
    Struct/System.cpp \
    Trends.cpp \
    WinDraw.cpp \
    WinFiles.cpp \
    WriteParams.cpp \
    WriteState.cpp \
    dialcolor.cpp \
    sysutils.cpp

HEADERS += \
    ../Include/BaseModel.h \
    ../Include/BaseType.h \
    ../Include/CommProc.h \
    ../Include/DB.h \
    ../Include/DataTypes.h \
    ../Include/Defects.h \
    ../Include/Err.h \
    ../Include/Extern_Pnt.h \
    ../Include/GDI.h \
    ../Include/PID.h \
    ../Include/PID_P.h \
    ../Include/Param.h \
    ../Include/SysDataTypes.h \
    ../Include/WinDraw.h \
    ../Include/WinFiles.h \
    ../Include/crossplatform.h \
    ../Include/sr.h \
    BaseModel.h \
    CommProc.h \
    Defects.h \
    Err.h \
    Param.h \
    Prop.h \
    PropsLoader.h \
    RsuX/RsuInfo.h \
    RsuX/RsuX.h \
    RsuX/RsuX_types.h \
    crossplatform.h \
    dialcolor.h \
    sysutils.h

