cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuDcuIO)
include (../../FlexCommon.cmake)

################################################################################
# Source groups
################################################################################
set(Headers
    "PrivateVars/RsuAI.h"
    "PrivateVars/RsuAO.h"
    "PrivateVars/RsuDI.h"
    "PrivateVars/RsuDO.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "FlexRsuDcuIO_dll.cpp"
    "RsuAI.cpp"
    "RsuAO.cpp"
    "RsuDI.cpp"
    "RsuDO.cpp"
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

set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "RsuX"
)

target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

