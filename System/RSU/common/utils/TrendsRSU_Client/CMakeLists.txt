project(TrendsRSU_Client)
set(CMAKE_FOLDER ".RSUs/common/utils")

include(../../common.cmake)

include_directories(
	"${CMAKE_SOURCE_DIR}/system/include/rsus/"
	"${CMAKE_SOURCE_DIR}/RSUs/common/include/"
)

set(Headers
    "TrendsRemoteClient.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "FillArray.cpp"
    "LoadPlugins.cpp"
    "trendsremoteclient.cpp"
    "trends_rsu_client_lib.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES ${Headers} ${Sources})
add_compile_definitions(TRENDSRSU_CLIENT_EXPORTS)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})
add_compile_definitions(RSU_CLIENT_LIB_EXPORT)

target_link_libraries(${PROJECT_NAME}
    PUBLIC TrendsKernel
    PUBLIC rsuSharedTrend_FlexRsu
    PRIVATE Utils
    PRIVATE crc32
)