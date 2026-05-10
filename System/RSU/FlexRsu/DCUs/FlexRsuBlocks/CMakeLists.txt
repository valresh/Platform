cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuBlocks)
include (../../FlexCommon.cmake)

set(Headers
    "../../include/BaseType_.h"
    "../../include/DataTypes_.h"
    "../../include/FlexBlocks/A2oo3.h"
    "../../include/FlexBlocks/ADDER.h"
    "../../include/FlexBlocks/COT.h"
    "../../include/FlexBlocks/DIO.h"
    "../../include/FlexBlocks/EV.h"
    "../../include/FlexBlocks/MOTOR.h"
    "../../include/FlexBlocks/PID.h"
    "../../include/FlexBlocks/PIDA.h"
    "../../include/FlexBlocks/RAMP.h"
    "../../include/FlexBlocks/SYSDATE.h"
    "../../include/FlexBlocks/SYSTIME.h"
    "../../include/FlexBlocks/TMR.h"
    "../../include/FlexBlocks/VLV.h"
    "../../include/FlexDeclareShared.h"
    "../../include/FlexPrivateDeclare.h"
    "../../include/SharedFlexBlocks.h"
    "../../include/_InitInfo.h"
    "../../include/FlexPrivateDeclare.h"
    "FlexRsuBlocks.h"
    "PrivateVars/ACalc.h"
	PrivateVars/ACalc8.h
    "PrivateVars/AConst.h"
    "PrivateVars/ADDER.h"
    "PrivateVars/And3.h"
    "PrivateVars/And5.h"
    "PrivateVars/ASel4.h"
    "PrivateVars/Copier.h"
    "PrivateVars/COT.h"
    "PrivateVars/DCI16.h"
    "PrivateVars/DCO16.h"
    "PrivateVars/DConst.h"
    "PrivateVars/DSel4.h"
    "PrivateVars/EV.h"
    "PrivateVars/FD2W.h"
    "PrivateVars/IREF.h"
    "PrivateVars/MOTOR.h"
    "PrivateVars/Not.h"
    "PrivateVars/Or3.h"
    "PrivateVars/Or5.h"
    "PrivateVars/OREF.h"
    "PrivateVars/PID.h"
    "PrivateVars/PIDA.h"
    "PrivateVars/RAMP.h"
    "PrivateVars/RS.h"
    "PrivateVars/SCLR.h"
    "PrivateVars/SGCR.h"
    "PrivateVars/SIMS.h"
    "PrivateVars/SR.h"
    "PrivateVars/TFw.h"
    "PrivateVars/TMR.h"
    "PrivateVars/TNw.h"
    "PrivateVars/TOFF.h"
    "PrivateVars/TON.h"
    "PrivateVars/TP.h"
    "PrivateVars/TW.h"
    "PrivateVars/VLV.h"
    "PrivateVars/WP.h"
	PrivateVars/ISINK.h
	PrivateVars/OSRC.h 
    "ScriptableBlock.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "A2oo3.cpp"
    "ACalc.cpp"
	ACalc8.cpp
    "AConst.cpp"
    "ADDER.cpp"
    "And3.cpp"
    "And5.cpp"
    "ASel4.cpp"
    "Copier.cpp"
    "COT.cpp"
    "DCI16.cpp"
    "DCO16.cpp"
    "DConst.cpp"
    "DSel4.cpp"
    "EV.cpp"
    "FD2W.cpp"
    "FlexRsuBlocks_dll.cpp"
    "IREF.cpp"
    "MOTOR.cpp"
    "Not.cpp"
    "Or3.cpp"
    "Or5.cpp"
    "OREF.cpp"
    "PID.cpp"
    "PIDA.cpp"
    "RAMP.cpp"
    "RS.cpp"
    "SCLR.cpp"
    "ScriptableBlock.cpp"
    "ScriptableBlock_Exec.cpp"
    "SGCR.cpp"
    "SIMS.cpp"
    "SR.cpp"
    "SYSDATE.cpp"
    "SYSTIME.cpp"
    "TFw.cpp"
    "TMR.cpp"
    "TNw.cpp"
    "TOFF.cpp"
    "TON.cpp"
    "TP.cpp"
    "TW.cpp"
    "VLV.cpp"
    "WP.cpp"
	ISINK.cpp
	OSRC.cpp 
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
    "../../../../utf8.manifest"
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(FLEXRSUBLOCKS_EXPORTS)

set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../FlexRsuCore/"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${COMMON_INCLUDES}
    ${PROJECT_INCLUDES}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE FlexRsuConnect
    PRIVATE FlexRsuCore
    PRIVATE FlexRsuTranslator
)

