cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/DCUs")

set(PROJECT_NAME rsuSharedTrend_SM5xx)
include(../../../../global.cmake)

set(Headers
	"rsuSharedTrend_SM5xx.h"
)
source_group("Headers" FILES ${Headers})

set(Sources	
	"rsuSharedTrend_SM5xx_dll.cpp"
)
source_group("Sources" FILES ${Sources})	

set(ALL_FILES
    ${Headers}
    ${Sources}
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(RSUSHAREDTREND_SM5XX_EXPORTS)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

target_link_libraries(${PROJECT_NAME} CommonRSUs crosslib RsuX UtilsSM5xx )

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

