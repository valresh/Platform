cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/DCUs")

set(PROJECT_NAME SafetyManagerH5xx_Client)
include(../../../../global.cmake)

set(Headers
	"ISMsPool.h"
	"SM_RemoteClient.h"
	"SMsPool.h"
)
source_group("Headers" FILES ${Headers})

set(Sources	
	"ACSsupport.cpp"
	"RsuConnectionImpl.cpp"
	"SafetyManagerH5xx_Client.cpp"
	"SM_RemoteClient.cpp"
	"SMsPool.cpp"
)
source_group("Sources" FILES ${Sources})	

set(ALL_FILES
    ${Headers}
    ${Sources}
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(SAFETYMANAGERH5XX_CLIENT_EXPORTS)
add_compile_definitions(RSU_CLIENT_LIB_EXPORT)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

target_link_libraries(${PROJECT_NAME} Utils CommonRSUs crosslib RsuX Connect ConnectSM5xx crc32)
if (WIN32)
target_link_libraries(${PROJECT_NAME} SafetyManagerH5xx_Debug)
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

