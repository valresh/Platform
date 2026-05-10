cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsu_Server)
include (../../FlexCommon.cmake)

set(Headers
    "../../include/IFlexStateSer.h"
    "FlexPool.h"
    "IFlexRsuModel.h"
    "Server.h"
    "StateSerLocal.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "FlexPool.cpp"
    "FlexRsu_Server_dll.cpp"
    "flex_server_lib.cpp"
    "Params.cpp"
    "Server.cpp"
    "Server_ACS.cpp"
    "States.cpp"
    "StateSerLocal.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
    "../../../../utf8.manifest"
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(RSU_SERVER_LIB_EXPORT)


set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../FlexRsuCore/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../FlexRsuImpl/"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${COMMON_INCLUDES}
    ${PROJECT_INCLUDES}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE FlexRsuImpl
    PRIVATE CommonRSUs
    PRIVATE crc32
)

add_dependencies (${PROJECT_NAME} CommonRSUs)
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "RsuX"
)

target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")


