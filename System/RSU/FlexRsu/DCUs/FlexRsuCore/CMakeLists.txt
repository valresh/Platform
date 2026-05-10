cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsuCore)
include (../../FlexCommon.cmake)

set(Headers
    "../../include/FlexVarInfo.h"
    "BlockCreator.h"
    "FlexBlockBase.h"
    "FlexConnection.h"
    "FlexDR.h"
    "FlexRsuCore.h"
	LibImplContainer.h
)
source_group("Headers" FILES ${Headers})

set(Sources
    "DynString.cpp"
    "BlockCreator.cpp"
    "FlexBlockBase.cpp"
    "FlexConnection.cpp"
    "FlexDR.cpp"
    "FlexRsuCore.cpp"
	LibImplContainer.cpp
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
    "../../../../utf8.manifest"
)

add_compile_definitions(FLEXRSUCORE_EXPORTS)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(FLEXRSUCORE_EXPORTS)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${COMMON_INCLUDES}
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE CommonRSUs 
    PRIVATE FlexRsuConnect 
)

