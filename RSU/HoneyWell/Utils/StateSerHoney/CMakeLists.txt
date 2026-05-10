cmake_minimum_required (VERSION 3.8) 
set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/Utils")

set(PROJECT_NAME StateSerHoney)
include(../../../../global.cmake)

set(Headers
	"StateSerHoney.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
	"StateSerHoney.cpp"
	"StateSer.cpp"
)
source_group("Sources" FILES ${Sources})	

set(ALL_FILES
    ${Headers}
    ${Sources}
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(STATESERHONEY_EXPORTS)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

target_link_libraries(${PROJECT_NAME} CommonRSUs crosslib CommonH Utils)

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

