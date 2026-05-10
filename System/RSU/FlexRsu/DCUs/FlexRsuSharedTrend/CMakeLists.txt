cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME rsuSharedTrend_FlexRsu)
include(../../FlexCommon.cmake)

set(Sources
    "rsuSharedTrend_FlexRsu.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Sources}
    "../../../../utf8.manifest"
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(RSU_SHARED_TREND_EXPORT)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${COMMON_INCLUDES}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC FlexRsuCommon
    PUBLIC FlexRsuList
)
