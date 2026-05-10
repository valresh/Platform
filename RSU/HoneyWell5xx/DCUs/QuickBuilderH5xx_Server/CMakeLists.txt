cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/DCUs")

set(PROJECT_NAME QuickBuilderH5xx_Server)
include(../../../../global.cmake)

set(Headers
	"QB_Server.h"
)
source_group("Headers" FILES ${Headers})

set(Sources	
	"QB_Params.cpp"
	"QB_Server.cpp"
	"QB_Server_ACS.cpp"
	"QB_States.cpp"
	"QuickBuilderH5xx_Server_dll.cpp"
	"RsuConnectionImpl.cpp"
)
source_group("Sources" FILES ${Sources})	

set(ALL_FILES
    ${Headers}
    ${Sources}
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(QUICKBUILDERH5XX_SERVER_EXPORTS)
add_compile_definitions(RSU_SERVER_LIB_EXPORT)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
add_dependencies (${PROJECT_NAME} MemoryServiceHCD)

target_link_libraries(${PROJECT_NAME} CommonRSUs crosslib RsuX ConnectH5xx QuickBuilderH StateSerHoneyLocal crc32)
if (WIN32)
target_link_libraries(${PROJECT_NAME} QuickBuilderH5xxDebug)
endif()

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

