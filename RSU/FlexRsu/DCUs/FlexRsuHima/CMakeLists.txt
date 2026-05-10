cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuHIMA)
include (../../FlexCommon.cmake)

################################################################################
# Source groups
################################################################################
set(Headers
    "F_TRIG.h"
    "PrivateVars/agt2.h"
    "PrivateVars/agt3.h"
    "PrivateVars/alt2.h"
    "PrivateVars/fts10s.h"
    "PrivateVars/fts20s.h"
    "PrivateVars/fts5s.h"
    "PrivateVars/qza.h"
    "RS.h"
    "SR.h"
    "TON.h"
    "TP.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "agt2.cpp"
    "agt3.cpp"
    "alt2.cpp"
    "F_TRIG.cpp"
    "FlexRsuHIMA_dll.cpp"
    "fts10s.cpp"
    "fts20s.cpp"
    "fts5s.cpp"
    "qza.cpp"
    "RS.cpp"
    "SR.cpp"
    "TON.cpp"
    "TP.cpp"
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
    CommonRSUs
)

