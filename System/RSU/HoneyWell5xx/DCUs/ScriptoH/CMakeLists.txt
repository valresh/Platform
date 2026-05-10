cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/DCUs")

set(PROJECT_NAME ScriptoH)
include(../../../../global.cmake)

set(Headers
	"export.h"
	"ScriptoH.h"
	"Trans.h"
)
source_group("Headers" FILES ${Headers})

set(Sources	
	"AnalizeTok.cpp"
	"Func.cpp"
	"Logic.cpp"
	"ScriptoH.cpp"
	"SET.cpp"
	"Tokens.cpp"
	"Trans.cpp"
	"Var.cpp"
)
source_group("Sources" FILES ${Sources})	

set(ALL_FILES
    ${Headers}
    ${Sources}
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(SCRIPTOH5XX_EXPORTS)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
add_dependencies (${PROJECT_NAME} MemoryServiceHCD)

target_link_libraries(${PROJECT_NAME} CommonRSUs crosslib RsuX UtilsQB5xx ConnectQB5xx TinyXML2 MemoryServiceQB5xx)

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

