cmake_minimum_required (VERSION 3.10)
set(CMAKE_FOLDER ".RSUs/common/utils")

set(PROJECT_NAME TinyXML2)
include(../../common.cmake)

set(Header_Files
    "../../include/TINYXML/tinyxml2.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "tinyxml2.cpp"
    "TinyXML2_dll.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
    "../../../../utf8.manifest"
)

add_compile_definitions(TINYXML2_EXPORT)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
