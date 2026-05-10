cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/Centum/Utils")

set(PROJECT_NAME YBridge2SysParam)
include(../../../../global.cmake)

set(Sources
	"YBridge2SysParam_dll"
)
source_group("Sources" FILES ${Sources})

set(Impl
	"../../Include/YBridge2SysParam.h"
	"YParam.cpp"
)
source_group("Impl" FILES ${Impl})	

set(ALL_FILES
    ${Sources}
    ${Impl}
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(YBRIDGE2SYSPARAM_EXPORTS)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

target_link_libraries(${PROJECT_NAME} Utils crosslib CommonRSUs)

set(PROJECT_INCLUDES
	"${CMAKE_CURRENT_SOURCE_DIR}/../../Include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../common/include"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_INCLUDES}
    ${COMMON_INCLUDES}
)

