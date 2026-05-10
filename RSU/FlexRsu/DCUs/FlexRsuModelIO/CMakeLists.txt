cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuModelIO)
include(../../FlexCommon.cmake)

set(Headers
    "../../include/FlexBlocks/AIO.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "AI.cpp"
    "AO.cpp"
    "DI.cpp"
    "DO.cpp"
    "FlexRsuModelIO_dll.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
    "../../../../utf8.manifest"
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../FlexRsuCore/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../FlexRsuList/"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${COMMON_INCLUDES}
    ${PROJECT_INCLUDES}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE FlexRsuConnect
    PRIVATE FlexRsuCore
)

