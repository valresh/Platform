cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuConnect)
include (../../FlexCommon.cmake)

set(Headers
    "../../include/FlexRsuConnect.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "FlexRsuConnect.cpp"
    "FlexRsuConnect_dll.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
    "../../../../utf8.manifest"
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(FLEXRSUCONNECT_EXPORTS)

set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../FlexRsuCore/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../FlexRsuList/"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_INCLUDES}
    ${COMMON_INCLUDES}
)
message(PROJECT_INCLUDES="${PROJECT_INCLUDES}")
message(COMMON_INCLUDES="${COMMON_INCLUDES}")

