cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuImpl)
include (../../FlexCommon.cmake)

set(Headers
    "../../include/BaseType_.h"
    "../../include/DataTypes_.h"
    "../../include/FlexVarTypes.h"
    "FlexRsu.h"
    "FlexRsuImpl_dll.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "FlexRsu.cpp"
    "FlexRsu_ACS.cpp"
    "FlexRsu_Calc.cpp"
    "FlexRsu_Init.cpp"
    "FlexRsu_ParamsSR.cpp"
    "FlexRsu_SRstate.cpp"
    "FlexRsuImpl_dll.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
    "../../../../utf8.manifest"
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(FLEXRSUIMPL_EXPORTS)

set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../FlexRsuCore/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../FlexRsuList/"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${COMMON_INCLUDES}
    ${PROJECT_INCLUDES}
)
message(COMMON_INCLUDES = "${COMMON_INCLUDES}")

if(NOT UNIX)
set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "FlexRsuDebugBridge"
)
endif()

target_link_libraries(${PROJECT_NAME} 
    PRIVATE FlexRsuCommon
    PRIVATE FlexRsuConnect
    PRIVATE FlexRsuCore
    PRIVATE TinyXML2
	PRIVATE cutf
    PUBLIC crosslib
    PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}"
)

