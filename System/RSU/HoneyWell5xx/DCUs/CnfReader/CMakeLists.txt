cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/HoneyWell5xx/DCUs")
set(PROJECT_NAME CnfReader)
include(../../../../global.cmake)

set(Headers
	"Block.h"
	"CnfReader.h"
	"Connection.h"
	"Coord.h"
	"IBlock.h"
	"ISymbolAttr.h"
	"SymbolAttr.h"
	"Vertex.h"
)
source_group("Headers" FILES ${Headers})

set(Sources	
	"Block.cpp"
	"CnfReader.cpp"
	"CnfReader_dll.cpp"
	"Coord.cpp"
	"SymbolAttr.cpp"
	"Vertex.cpp"
)
source_group("Sources" FILES ${Sources})	

set(ALL_FILES
    ${Headers}
    ${Sources}
	"../../../../utf8.manifest"
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(CNFREADER_EXPORTS)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
add_dependencies (${PROJECT_NAME} MemoryServiceHCD)

target_link_libraries(${PROJECT_NAME} Utils CommonRSUs crosslib RsuX TinyXML2)
if (WIN32)
target_link_libraries(${PROJECT_NAME} CnfReaderUI)
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

