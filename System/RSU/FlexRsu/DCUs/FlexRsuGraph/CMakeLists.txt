cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuGraph)
include (../../FlexCommon.cmake)

################################################################################
# Source groups
################################################################################
set(Headers
    "../../include/FlexBlocks/AnaTrend.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "AnaTrend.cpp"
    "FlexRsuGraph_dll.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

################################################################################
# Include directories
################################################################################
set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../FlexRsuCore/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../FlexRsuList/"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${COMMON_INCLUDES}
    ${PROJECT_INCLUDES}
)

################################################################################
# Dependencies
################################################################################
# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    FlexRsuConnect
    FlexRsuCore
)

