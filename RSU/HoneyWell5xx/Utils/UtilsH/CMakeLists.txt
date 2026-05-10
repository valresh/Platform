cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/Utils")

set(PROJECT_NAME UtilsH5xx)
include(../../../../global.cmake)

set(Headers
	"ListValue.h"
	"Sh_Sheme.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "BuildLists.cpp"
    "CBHSystem.cpp"
	"HoneywellStruct.cpp"
	"ListValue.cpp"
	"OneMnemoHoney.cpp"
#	"StateSer.cpp"
	"UtilsH.cpp"
	"OneAlmGrHoney.cpp"
)
source_group("Sources" FILES ${Sources})	

set(ALL_FILES
    ${Headers}
    ${Sources}
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(UTILSH5XX_EXPORTS)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

target_link_libraries(${PROJECT_NAME} CommonRSUs crosslib CommonH Utils)

set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"        
	"${CMAKE_CURRENT_SOURCE_DIR}/../../Include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/rsus"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../common/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../HoneyWell/Include"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_INCLUDES}
    ${COMMON_INCLUDES}
)

