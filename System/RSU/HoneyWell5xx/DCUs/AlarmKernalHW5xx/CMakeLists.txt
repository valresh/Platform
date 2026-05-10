cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/DCUs")

set(PROJECT_NAME AlarmKernalHW5xx)
include(../../../../global.cmake)

set(Headers
	"AlarmKernalHW5xx.h"
	"AlarmsFile.h"
	"AlarmsW.h"
	"Mnemo.h"
	"AlmGr.h"
)
source_group("Headers" FILES ${Headers})

set(Sources	
	"AlarmKernalHW5xx.cpp"
	"AlarmKernalHW5xx_dll.cpp"
	"AlarmsFile.cpp"
	"AlarmsW.cpp"
	"Mnemo.cpp"
	"AlmGr.cpp"
)
source_group("Sources" FILES ${Sources})	

set(ALL_FILES
    ${Headers}
    ${Sources}
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(ALARMKERNALHW5XX_EXPORTS)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
add_dependencies (${PROJECT_NAME} MemoryServiceHCD)

target_link_libraries(${PROJECT_NAME} Utils CommonH CommonRSUs crosslib RsuX UtilsH5xx ConnectH5xx MemoryServiceH5xx TinyXML2)

set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"        
	"${CMAKE_CURRENT_SOURCE_DIR}/../../Include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/rsus"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../common/include"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_INCLUDES}
    ${COMMON_INCLUDES}
)

