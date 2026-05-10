cmake_minimum_required (VERSION 3.10)
set(CMAKE_FOLDER ".RSUs/common/utils")

set(PROJECT_NAME XmlMarker)
include(../../common.cmake)

set(Header_Files
    "../../include/IXmlNode.h"
    "XmlMarker.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Impl
    "XmlMaker.h"
    "XmlNode.h"
)
source_group("Impl" FILES ${Impl})

set(Source_Files
    "XmlMarker.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Impl}
    ${Source_Files}
    "../../../../utf8.manifest"
)

add_compile_definitions(XMLMARKER_EXPORTS)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
