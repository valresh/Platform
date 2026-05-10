cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/Centum/Utils")

set(PROJECT_NAME CommonY)
include(../../../../global.cmake)

set(Sources
    "CommonY-dll.cpp"
)
source_group("Sources" FILES ${Sources})	

set(Impl
    "CommonY.cpp"
	"../../Include/CommonY.h"
)
source_group("Impl" FILES ${Impl})	

set(ALL_FILES
    ${Sources}
    ${Impl}
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(COMMONY_EXPORTS)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
#add_dependencies (${PROJECT_NAME} )

target_link_libraries(${PROJECT_NAME} Utils crosslib CommonRSUs)

set(PROJECT_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/"        
	"${CMAKE_CURRENT_SOURCE_DIR}/../../Include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../common/include"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_INCLUDES}
    ${COMMON_INCLUDES}
)

