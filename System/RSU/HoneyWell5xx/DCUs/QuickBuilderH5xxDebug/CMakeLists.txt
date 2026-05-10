cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/DCUs")

set(PROJECT_NAME QuickBuilderH5xxDebug)
include(../../../../global.cmake)

set(Headers
	"Dial_Value.h"
	"GridCtrl.h"
	"InPlaceCombo.h"
	"InPlaceEdit.h"
	"QuickBuilderApp.h"
	"QuickBuilderH5xxDebug.h"
	"Resource.h"
	"stdafx.h"
)
source_group("Headers" FILES ${Headers})

set(Sources	
	"Dial_Value.cpp"
	"GridCtrl.cpp"
	"InPlaceCombo.cpp"
	"InPlaceEdit.cpp"
	"QuickBuilderApp.cpp"
	"QuickBuilderH5xxDebug_dll.cpp"
	"stdafx.cpp"
)
source_group("Sources" FILES ${Sources})	

set(ALL_FILES
    ${Headers}
    ${Sources}
	"../../../../utf8.manifest"
	"QuickBuilderH5xxDebug.rc"
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(_AFXDLL)
add_compile_definitions(QUICKBUILDERH5XXDEBUG_EXPORTS)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
add_dependencies (${PROJECT_NAME} MemoryServiceHCD)

target_link_libraries(${PROJECT_NAME} CommonRSUs crosslib RsuX UtilsH5xx ConnectH5xx TinyXML2 MemoryServiceH5xx CnfReader)

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

