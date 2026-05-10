cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuAlarmsKernel)
include (../../FlexCommon.cmake)

set(Headers
	"AlarmsFile.h"
	"AlarmsFlex.h"
	"FlexRsuAlarmsKernel.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
	"AlarmsFile.cpp"
	"AlarmsFlex.cpp"
	"FlexRsuAlarmsKernel_dll.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
    "../../../../utf8.manifest"
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})
add_compile_definitions(FLEXRSUALARMSKERNEL_EXPORTS)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC FlexRsuCommon CommonRSUs
    PRIVATE Utils
    PRIVATE crc32
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../crosslib/include/"
)

