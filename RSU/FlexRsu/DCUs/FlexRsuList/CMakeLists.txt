cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuList)
include (../../FlexCommon.cmake)

set(Headers
    "FlexRsuList.h"
    "SharedListValue.h"
    "SharedListValue_Build.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "A2oo3.cpp"
    "ADDER.cpp"
    "AIO.cpp"
    "AnAlrm.cpp"
    "AnaTrend.cpp"
    "COT.cpp"
    "DigAlrm.cpp"
    "DIO.cpp"
    "EV.cpp"
    "FlexRsuList_dll.cpp"
    "FutureTrend.cpp"
    "M2U.cpp"
    "MOTOR.cpp"
    "OpcAIO.cpp"
    "OpcDIO.cpp"
    "PID.cpp"
    "PIDA.cpp"
    "RAMP.cpp"
    "SharedListValue.cpp"
    "SYSDATE.cpp"
    "SYSTIME.cpp"
    "TCoreDAIO.cpp"
    "TMR.cpp"
    "TXTSET.cpp"
    "VLV.cpp"
    "PidSKVO.cpp"
    "SCLR.cpp"
    "Sim_DIO.cpp"
    "Sim_AIO.cpp"
    "DCI16.cpp"
    "DCO16.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
    "../../../../utf8.manifest"
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(FLEXRSULIST_EXPORTS)

target_link_libraries(${PROJECT_NAME} CommonRSUs)

set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../FlexRsuCore/"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${COMMON_INCLUDES}
    ${PROJECT_INCLUDES}
)


