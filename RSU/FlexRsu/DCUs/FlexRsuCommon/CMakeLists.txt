cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuCommon)
include(${CMAKE_CURRENT_LIST_DIR}/../../FlexCommon.cmake)

set(Headers
    "../../include/FlexRsuCommon.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "FlexRsuCommon.cpp"
    "FlexRsuCommon_dll.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
    "../../../../utf8.manifest"
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(FLEXRSUCOMMON_EXPORTS)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

add_dependencies (${PROJECT_NAME} CommonRSUs)
target_link_libraries(${PROJECT_NAME} PRIVATE CommonRSUs)

set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"        
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_INCLUDES}
    ${COMMON_INCLUDES}
)


