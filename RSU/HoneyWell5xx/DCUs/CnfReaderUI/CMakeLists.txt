cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/DCUs")
set(PROJECT_NAME CnfReaderUI)
include(../../../../global.cmake)

set(Headers
	"CnfReaderUI.h"
	"ProgressDlg.h"
	"Resource.h"
	"stdafx.h"
)
source_group("Headers" FILES ${Headers})

set(Sources	
	"CnfReaderUI.cpp"
	"ProgressDlg.cpp"
	"stdafx.cpp"
)
source_group("Sources" FILES ${Sources})	

set(ALL_FILES
    ${Headers}
    ${Sources}
	"../../../../utf8.manifest"
	"CnfReaderUI.rc"
)
#add_executable(${PROJECT_NAME} WIN32 ${ALL_FILES})
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(CNFREADERUI_EXPORTS)
add_compile_definitions(_AFXDLL)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
add_dependencies (${PROJECT_NAME} MemoryServiceHCD)

target_link_libraries(${PROJECT_NAME} CommonRSUs crosslib RsuX TinyXML2 )

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

