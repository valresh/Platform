cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuAlarms_Client)
include (../../FlexCommon.cmake)

set(Headers
	"AlarmClient.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "AlarmClient.cpp"
    "Alarms_Client_dll.cpp"
    "flex_alarms_client_lib.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
    "../../../../utf8.manifest"
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(RSU_CLIENT_LIB_EXPORT)


target_link_libraries(${PROJECT_NAME} 
    PUBLIC FlexRsuCommon
    PUBLIC FlexRsuAlarmsKernel
    PRIVATE Utils
    PRIVATE crc32
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../crosslib/include/"
)

