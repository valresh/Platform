cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuAlarms_Server)
include (../../FlexCommon.cmake)

set(Headers
	"AlarmServer.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
	"Alarms_Server_dll.cpp"
	"AlarmServer.cpp"
	"flex_alarms_server_lib.cpp"
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
    PRIVATE FlexRsuConnect
    PRIVATE FlexRsuAlarmsKernel
    PRIVATE CommonRSUs
    PRIVATE crc32
    PUBLIC Utils
)

target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")


