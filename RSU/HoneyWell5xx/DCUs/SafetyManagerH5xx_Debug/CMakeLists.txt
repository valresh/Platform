cmake_minimum_required (VERSION 3.10)
set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/DCUs")

set(PROJECT_NAME SafetyManagerH5xx_Debug)
include(../../../../global.cmake)

set(Headers
	"CbhList.h"
	"ComList.h"
	"EditFix.h"
	"FldComList.h"
	"FldList.h"
	"resource.h"
	"SafetyManagerApp.h"
	"SafetyManagerDebug_dll.h"
	"SafetyManagerDoc.h"
	"SafetyManagerFrame.h"
	"SafetyManagerView.h"
	"stdafx.h"
	"ViewList.h"
)
source_group("Headers" FILES ${Headers})

set(Sources	
	"CbhList.cpp"
	"ComList.cpp"
	"EditFix.cpp"
	"FldComList.cpp"
	"FldList.cpp"
	"SafetyManagerApp.cpp"
	"SafetyManagerDebug_dll.cpp"
	"SafetyManagerDoc.cpp"
	"SafetyManagerFrame.cpp"
	"SafetyManagerView.cpp"
	"stdafx.cpp"
	"ViewList.cpp"
)
source_group("Sources" FILES ${Sources})	

set(ALL_FILES
    ${Headers}
    ${Sources}
	"../../../../utf8.manifest"
	"SafetyManagerH5xx_Debug.rc"
)
#add_executable(${PROJECT_NAME} WIN32 ${ALL_FILES})
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(_AFXDLL)
add_compile_definitions(SAFETYMANAGERH5XX_DEBUG_EXPORTS)

target_link_libraries(${PROJECT_NAME} CommonRSUs crosslib RsuX UtilsH5xx ConnectH5xx MemoryServiceH5xx)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"        
	"${CMAKE_CURRENT_SOURCE_DIR}/../../Include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/rsus"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../common/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../SafetyManagerH5xx_Server"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_INCLUDES}
    ${COMMON_INCLUDES}
)

