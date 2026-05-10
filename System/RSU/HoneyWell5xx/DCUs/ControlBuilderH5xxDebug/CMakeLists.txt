cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/DCUs")

set(PROJECT_NAME ControlBuilderH5xxDebug)
include(../../../../global.cmake)

set(HeadersLin
	"ControlBuilderH5xxDebug.h"
)
source_group("HeadersLin" FILES ${HeadersLin})

set(Headers
	"BlkDraw.h"
	"ControlBuilderApp.h"
	"controlbuilderdoc.h"
	"ControlBuilderH5xxDebug.h"
	"ControlBuilderSimpleFrame.h"
	"ControlBuilderView.h"
	"Dial_Value.h"
	"GridCtrl.h"
	"InPlaceCombo.h"
	"InPlaceEdit.h"
	"PrjView.h"
	"Resource.h"
	"stdafx.h"
)
source_group("Headers" FILES ${Headers})

set(SourcesLin	
	"HParamInfo.cpp"
)
source_group("SourcesLin" FILES ${SourcesLin})	

set(Sources	
	"BlkDraw.cpp"
	"ControlBuilderApp.cpp"
	"ControlBuilderDoc.cpp"
	"ControlBuilderH5xxDebug_dll.cpp"
	"ControlBuilderSimpleFrame.cpp"
	"ControlBuilderView.cpp"
	"Dial_Value.cpp"
	"GridCtrl.cpp"
	"HParamInfo.cpp"
	"InPlaceCombo.cpp"
	"InPlaceEdit.cpp"
	"PrjView.cpp"
	"stdafx.cpp"
)
source_group("Sources" FILES ${Sources})	
if (UNIX)
set(ALL_FILES
    ${HeadersLin}
    ${SourcesLin}
)
else()
set(ALL_FILES
    ${Headers}
    ${Sources}
	"../../../../utf8.manifest"
	"ControlBuilderH5xxDebug.rc"
)
endif()
#add_executable(${PROJECT_NAME} WIN32 ${ALL_FILES})
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(_AFXDLL)
add_compile_definitions(CONTROLBUILDERH5XXDEBUG_EXPORTS)

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

