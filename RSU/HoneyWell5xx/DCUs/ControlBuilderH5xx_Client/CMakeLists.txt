cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/DCUs")

set(PROJECT_NAME ControlBuilderH5xx_Client)
include(../../../../global.cmake)

set(Headers
	"CBsPool.h"
	"H_RemoteClient.h"
	"ICBsPool.h"
)
source_group("Headers" FILES ${Headers})

set(Sources	
	"ACSsupport.cpp"
	"CBsPool.cpp"
	"ControlBuilderH5xx_Client.cpp"
	"H_RemoteClient.cpp"
	"RsuConnectionImpl.cpp"
)
source_group("Sources" FILES ${Sources})	

set(ALL_FILES
    ${Headers}
    ${Sources}
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(CONTROLBUILDERH5XX_CLIENT_EXPORTS)
add_compile_definitions(RSU_CLIENT_LIB_EXPORT)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
add_dependencies (${PROJECT_NAME} MemoryServiceHCD)

target_link_libraries(${PROJECT_NAME} PRIVATE Utils UtilsH5xx CommonRSUs crosslib RsuX ConnectH5xx Connect crc32)

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

