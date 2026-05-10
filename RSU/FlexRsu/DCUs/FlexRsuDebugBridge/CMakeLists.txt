cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuDebugBridge)
include (../../FlexCommon.cmake)

set(Headers
    	"DebugApp.h"
	"DrsView.h"
	"FieldsView.h"
	"FlexDoc.h"
	"FlexFrame.h"
	"FlexRsuDebugBridge.h"
	"ListOfBlocksView.h"
	"MyTreeCtrl.h"
	"OpenInGraph.h"
	"resource.h"
	"stdafx.h"
	"TreeListView.h"
	FindByName.h
)
source_group("Headers" FILES ${Headers})

set(Sources
	"DebugApp.cpp"
	"DrsView.cpp"
	"FieldsView.cpp"
	"FlexDoc.cpp"
	"FlexFrame.cpp"
	"FlexRsuDebugBridge.cpp"
	"ListOfBlocksView.cpp"
	"MyTreeCtrl.cpp"
	"OpenInGraph.cpp"
	"stdafx.cpp"
	"TreeListView.cpp"
	FindByName.cpp
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
	"../../../../utf8.manifest"
	"FlexRsuDebugBridge.rc"
)

add_compile_definitions(_AFXDLL)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(FLEXRSUDEBUGBRIDGE_EXPORTS)

target_link_libraries(${PROJECT_NAME} PUBLIC
    FlexRsuCommon
    FlexRsuList
	FlexRsuCore
	crosslib
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "Utils"
        "RsuX"
)

target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../crosslib/include/"
)

