cmake_minimum_required (VERSION 3.10)
set(PROJECT_NAME FlexRsu_ServerExSharedPlugin)
include (../../FlexCommon.cmake)

set(Headers
    "../../../common/include/ServerExPluginTypes.h"
    "RsuF.h"
    "ServerExSharedPlugin_FlexRsu.h"
	TagSkipFilter.h
)
source_group("Headers" FILES ${Headers})

set(Sources
    "AlarmsExt.cpp"
    "FindConnPointEx.cpp"
    "MessagesExt.cpp"
    "PutModel.cpp"
    "ServerExSharedPlugin_FlexRsu_dll.cpp"
	TagSkipFilter.cpp
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
    "../../../../utf8.manifest"
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(SERVEREXSHAREDPLUGIN_FLEXRSU_EXPORTS)


set(PROJECT_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../include/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../crosslib/include/"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${COMMON_INCLUDES}
    ${PROJECT_INCLUDES}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE FlexRsuCommon
    PRIVATE FlexRsuList
    PRIVATE Utils
)

add_dependencies(${PROJECT_NAME} FlexRsuCommon FlexRsuList)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "RsuX"
)

target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")


