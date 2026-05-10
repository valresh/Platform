cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuSimIO)
include (../../FlexCommon.cmake)

################################################################################
# Source groups
################################################################################
set(Impl
    "SimAI.cpp"
	"SimAO.cpp"
	"SimDI.cpp"
	"SimDO.cpp"
)
source_group("Impl" FILES ${Impl})

set(Sources
    "FlexRsuSimIO_dll.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Impl}
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

target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

